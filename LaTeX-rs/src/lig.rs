use std::{collections::HashMap, fmt::Display};
use std::io::{Write, Error as IOError};

pub trait LatexElement {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write;
}

#[derive(PartialEq, Eq, Clone, Debug, Hash)]
pub enum StrLigature<'a> {
    Own(String),
    Ref(&'a str)
}
impl From<String> for StrLigature<'static> {
    fn from(value: String) -> Self {
        Self::Own(value)
    }
}
impl<'a> From<&'a str> for StrLigature<'a> {
    fn from(value: &'a str) -> Self {
        Self::Ref(value)
    }
}
impl Display for StrLigature<'_> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let x: &dyn Display = match self {
            Self::Own(v) => v,
            Self::Ref(v) => v
        };

        x.fmt(f)
    }
}
impl LatexElement for StrLigature<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        let data: &[u8] = match self {
            Self::Own(v) => v.as_bytes(),
            Self::Ref(v) => v.as_bytes()
        };

        to.write_all(data)
    }
}

macro_rules! impl_num_ligature {
    ( $( $src:ty => $variant:ident ),* $(,)? ) => {
        #[derive(PartialEq, Clone, Copy, Debug)]
        pub enum NumLigature {
            $($variant($src),)*
        }

        $(
            impl From<$src> for NumLigature {
                fn from(value: $src) -> Self {
                    Self::$variant(value)
                }
            }
        )*

        impl Display for NumLigature {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                let x: &dyn Display = match self {
                    $(Self::$variant(v) => v,)*
                };

                x.fmt(f)
            }
        }
    }
}

impl_num_ligature! {
    i8 => Int8,
    i16 => Int16,
    i32 => Int32,
    i64 => Int64,
    u8 => UInt8,
    u16 => UInt16,
    u32 => UInt32,
    u64 => UInt64,
    usize => USize,
    isize => ISize,
    f32 => Float,
    f64 => Double
}

impl LatexElement for NumLigature {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        let as_string = self.to_string();
        let bytes = as_string.as_bytes();

        to.write_all(bytes)
    }
}

#[derive(Clone, PartialEq, Eq, Debug, Hash)]
pub struct SingularCommandLigature<'a> {
    name: StrLigature<'a>
}
impl LatexElement for SingularCommandLigature<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        to.write_all(b"\\")?;
        self.name.write(to)
    }
}
impl<'a> SingularCommandLigature<'a> {
    pub fn new<S>(name: S) -> Self 
    where S: Into<StrLigature<'a>> {
        Self {
            name: name.into()
        }
    }
}

#[derive(Clone, PartialEq, Debug)]
pub enum CommandFragment<'a> {
    Str(StrLigature<'a>),
    Num(NumLigature)
}
impl<'a> From<StrLigature<'a>> for CommandFragment<'a> {
    fn from(value: StrLigature<'a>) -> Self {
        Self::Str(value)
    }
}
impl From<NumLigature> for CommandFragment<'static> {
    fn from(value: NumLigature) -> Self {
        Self::Num(value)
    }
}
impl LatexElement for CommandFragment<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        match self {
            Self::Str(x) => x.write(to),
            Self::Num(x) => x.write(to)
        }
    }
}

#[derive(Clone, PartialEq, Debug)]
pub enum CommandArgument<'a> {
    Frag(CommandFragment<'a>),
    Vec(Vec<CommandFragment<'a>>),
    Map(HashMap<StrLigature<'a>, CommandFragment<'a>>)
}
impl LatexElement for CommandArgument<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        match self {
            Self::Frag(x) => x.write(to),
            Self::Vec(vec) => {
                if vec.is_empty() {
                    return Ok( () );
                }

                let last_index = vec.len() - 1;
                for (i, frag) in vec.iter().enumerate() {
                    frag.write(to)?;
                    if i != last_index {
                        to.write_all(b",")?;
                    }
                }

                Ok( () )
            },
            Self::Map(map) => {
                if map.is_empty() {
                    return Ok( () );
                }

                let last_index = map.len() - 1;
                for (i, (name, frag)) in map.iter().enumerate() {
                    name.write(to)?;
                    to.write_all(b"=")?;
                    frag.write(to)?;

                    if i != last_index {
                        to.write_all(b",")?;
                    }
                }

                Ok( () )
            }
        }
    }
}

pub struct RelationalCommandArgumentBuilder<'a> {
    map: HashMap<StrLigature<'a>, CommandFragment<'a>>
}
impl<'a> RelationalCommandArgumentBuilder<'a> {
    pub fn new() -> Self {
        Self {
            map: HashMap::new()
        }
    }
    
    pub fn add_relation<S, V>(&mut self, key: S, value: V) -> &mut Self
    where S: Into<StrLigature<'a>>,
    V: Into<CommandFragment<'a>> {
        self.map.insert(
            key.into(),
            value.into()
        );

        self
    }

    pub fn build(self) -> CommandArgument<'a> {
        CommandArgument::Map(self.map)
    }
    pub fn build_direct(self) -> HashMap<StrLigature<'a>, CommandFragment<'a>> {
        self.map
    }
}

#[derive(Clone, PartialEq, Debug)]
pub enum LigatureFragment<'a> {
    Str(StrLigature<'a>),
    Num(NumLigature),
    SngCmd(SingularCommandLigature<'a>),
    Vec(Vec<LigatureFragment<'a>>)
}
impl<'a> From<&'a str> for LigatureFragment<'a> {
    fn from(value: &'a str) -> Self {
        Self::Str(StrLigature::Ref(value))
    }
}
impl From<String> for LigatureFragment<'static> {
    fn from(value: String) -> Self {
        Self::Str(StrLigature::Own(value))
    }
}
impl<'a> From<StrLigature<'a>> for LigatureFragment<'a> {
    fn from(value: StrLigature<'a>) -> Self {
        Self::Str(value)
    }
}
impl<'a> From<NumLigature> for LigatureFragment<'a> {
    fn from(value: NumLigature) -> Self {
        Self::Num(value)
    }
}
impl<'a> From<SingularCommandLigature<'a>> for LigatureFragment<'a> {
    fn from(value: SingularCommandLigature<'a>) -> Self {
        Self::SngCmd(value)
    }
}
impl<'a> From<Vec<LigatureFragment<'a>>> for LigatureFragment<'a> {
    fn from(value: Vec<LigatureFragment<'a>>) -> Self {
        Self::Vec(value)
    }
}
impl LatexElement for LigatureFragment<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        match self {
            Self::Str(x) => x.write(to),
            Self::Num(x) => x.write(to),
            Self::SngCmd(x) => x.write(to),
            Self::Vec(vec) => {
                for frag in vec {
                    frag.write(to)?;
                }

                Ok( () )
            }
        }
    }
}
impl<'a> LigatureFragment<'a> {
    pub fn new_iter<I>(iter: I) -> Self 
    where I: IntoIterator<Item = LigatureFragment<'a>> {
        Self::Vec(iter.into_iter().collect())
    }
}

#[derive(Clone, PartialEq, Debug)]
pub enum Line<'a> {
    Frag(LigatureFragment<'a>),
    InlineMath(LigatureFragment<'a>),
    Cmd(Box<Command<'a>>),
    Blank,
    Comment(LigatureFragment<'a>)
}
impl<'a> From<LigatureFragment<'a>> for Line<'a> {
    fn from(value: LigatureFragment<'a>) -> Self {
        Self::Frag(value)
    }
}
impl<'a> From<Command<'a>> for Line<'a> {
    fn from(value: Command<'a>) -> Self {
        Self::Cmd(Box::new(value))
    }
}
impl<'a> From<Box<Command<'a>>> for Line<'a> {
    fn from(value: Box<Command<'a>>) -> Self {
        Self::Cmd(value)
    }
}
impl LatexElement for Line<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        match self {
            Self::Frag(x) => x.write(to),
            Self::InlineMath(x) => {
                to.write_all(b"$")?;
                x.write(to)?;
                to.write_all(b"$")
            },
            Self::Cmd(x) => x.write(to),
            Self::Blank => Ok( () ),
            Self::Comment(l) => {
                to.write_all(b"% ")?;
                l.write(to)
            }
        }
    }
}
impl<'a> Line<'a> {
    pub fn new_math<F>(frag: F) -> Self 
    where F: Into<LigatureFragment<'a>> {
        Self::InlineMath(frag.into())
    }

    pub fn new_comment<F>(frag: F) -> Self 
    where F: Into<LigatureFragment<'a>> {
        Self::Comment(frag.into())
    }
}

// Since 'Simple' and 'Complex' command would get wrapped in an enum, it defeats the purpose of
// having two different types.

#[derive(Clone, PartialEq, Debug)]
struct CommandDecl<'a> {
    name: StrLigature<'a>,
    post_args_bracket: Option<CommandArgument<'a>>,
    post_args: Option<CommandArgument<'a>>
}
impl<'a> CommandDecl<'a> {
    fn new<S>(name: S) -> Self 
    where S: Into<StrLigature<'a>> {
        Self {
            name: name.into(),
            post_args_bracket: None,
            post_args: None
        }
    }
}

#[derive(Clone, PartialEq, Debug)]
pub struct CommandDeclBuilder<'a> {
    inner: CommandDecl<'a> 
}
impl<'a> CommandDeclBuilder<'a> {
    fn new<S>(name: S) -> Self 
    where S: Into<StrLigature<'a>> {
        Self {
            inner: CommandDecl::new(name)
        }
    }

    pub fn with_post_args_bracket<T>(mut self, args: T) -> Self 
    where T: Into<CommandArgument<'a>>{
        self.inner.post_args_bracket = Some(args.into());
        self
    }
    pub fn without_post_args_bracket(mut self) -> Self {
        self.inner.post_args_bracket = None;
        self
    }
    pub fn with_post_args<T>(mut self, args: T) -> Self 
    where T: Into<CommandArgument<'a>> {
        self.inner.post_args = Some(args.into());
        self
    }
    pub fn without_post_args(mut self) -> Self {
        self.inner.post_args = None;
        self
    }

    fn build(self) -> CommandDecl<'a> {
        self.inner
    }
}

#[derive(Clone, PartialEq, Debug)]
pub struct Command<'a> {
    decl: CommandDecl<'a>,
    pre_args: Option<CommandArgument<'a>>,
    content: Line<'a>,
}
impl LatexElement for Command<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        to.write_all(b"\\")?;
        self.decl.name.write(to)?;
        if let Some(pre_args) = self.pre_args.as_ref() {
            to.write_all(b"[")?;
            pre_args.write(to)?;
            to.write_all(b"]")?;
        }
        
        to.write_all(b"{")?;
        self.content.write(to)?;
        to.write_all(b"}")?;

        if let Some(post_args) = self.decl.post_args_bracket.as_ref() {
            to.write_all(b"[")?;
            post_args.write(to)?;
            to.write_all(b"]")?;
        }
        
        if let Some(post_args) = self.decl.post_args.as_ref() {
            to.write_all(b"{")?;
            post_args.write(to)?;
            to.write_all(b"}")?;
        }

        Ok( () )
    }
}
impl<'a> Command<'a> {
    pub fn new<T, S>(name: S, content: T, pre_args: Option<CommandArgument<'a>>) -> Self 
    where T: Into<Line<'a>>,
    S: Into<StrLigature<'a>> {
        Self {
            decl: CommandDecl::new(name),
            pre_args,
            content: content.into()
        }
    }
    pub fn new_builder<T, S, F>(name: S, build: F, content: T, pre_args: Option<CommandArgument<'a>>) -> Self 
    where F: FnOnce(CommandDeclBuilder<'a>) -> CommandDeclBuilder<'a>,
    T: Into<Line<'a>>,
    S: Into<StrLigature<'a>> {
        let decl = build( CommandDeclBuilder::new(name) ).build();
        Self {
            decl,
            pre_args,
            content: content.into()
        }
    }
}

#[derive(Clone, PartialEq, Debug)]
pub struct BlockCommand<'a> {
    decl: CommandDecl<'a>,
    content: Vec<Block<'a>>,
    with_line_ends: bool
}
impl LatexElement for BlockCommand<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        to.write_all(b"\\begin{")?;
        self.decl.name.write(to)?;
        to.write_all(b"}")?;    

        if let Some(post_args) = self.decl.post_args_bracket.as_ref() {
            to.write_all(b"[")?;
            post_args.write(to)?;
            to.write_all(b"]")?;
        }    
 
        if let Some(post_args) = self.decl.post_args.as_ref() {
            to.write_all(b"{")?;
            post_args.write(to)?;
            to.write_all(b"}")?;
        }

        to.write_all(b"\n")?;

        if !self.content.is_empty() {
            let last_index = self.content.len() - 1;
            for (i, line) in self.content.iter().enumerate() {
                to.write_all(b"\t")?;
                line.write(to)?;

                if self.with_line_ends && i != last_index {
                    to.write_all(b"\\\\")?;
                }

                to.write_all(b"\n")?;
            }
        }

        to.write_all(b"\\end{")?;
        self.decl.name.write(to)?;
        to.write_all(b"}")?;

        Ok( () )
    }
}
impl<'a> BlockCommand<'a> {
    pub fn new<S>(name: S, content: Vec<Block<'a>>, with_line_ends: bool) -> Self 
    where S: Into<StrLigature<'a>> {
        Self {
            decl: CommandDecl::new(name),
            content,
            with_line_ends
        }
    }
    pub fn new_builder<S, F>(name: S, build: F, content: Vec<Block<'a>>, with_line_ends: bool) -> Self 
    where F: FnOnce(CommandDeclBuilder<'a>) -> CommandDeclBuilder<'a>,
    S: Into<StrLigature<'a>> {
        let builder = CommandDeclBuilder::new(name);
        let built_decl = build(builder).build();

        Self {
            decl: built_decl,
            content,
            with_line_ends
        }
    }
    pub fn new_iter<S, I>(name: S, content: I, with_line_ends: bool) -> Self 
    where I: IntoIterator<Item = Block<'a>>,
    S: Into<StrLigature<'a>> {
        let collected: Vec<Line<'a>> = content.into_iter().collect();

        Self {
            decl: CommandDecl::new(name),
            content: collected,
            with_line_ends
        }
    }
    pub fn new_iter_builder<S, I, F>(name: &'static str, build: F, content: I, with_line_ends: bool) -> Self 
    where I: IntoIterator<Item = Block<'a>>,
    F: FnOnce(CommandDeclBuilder<'a>) -> CommandDeclBuilder<'a>,
    S: Into<StrLigature<'a>> {
        let collected: Vec<Line<'a>> = content.into_iter().collect();

        let builder = CommandDeclBuilder::new(name);
        let built_decl = build(builder).build();

        Self {
            decl: built_decl,
            content: collected,
            with_line_ends
        }
    }
}

#[derive(Clone, PartialEq, Debug)]
pub struct MathBlock<'a> {
    content: LigatureFragment<'a>
}
impl LatexElement for MathBlock<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        to.write_all(b"\\[\n\t")?;
        self.content.write(to)?;
        to.write_all(b"\n\\]")?;
        Ok( () )
    }
}
impl<'a> MathBlock<'a> {
    pub fn new<T>(content: T) -> Self where T: Into<LigatureFragment<'a>> {
        Self {
            content: content.into()
        }
    }

    pub fn content(&self) -> &LigatureFragment<'a> {
        &self.content
    }
}

#[derive(Clone, PartialEq, Debug)]
pub enum Block<'a> {
    Line(Line<'a>),
    Cmd(BlockCommand<'a>),
    Math(MathBlock<'a>)
}
impl LatexElement for Block<'_> {
    fn write<W>(&self, to: &mut W) -> Result<(), IOError> where W: Write {
        match self {
            Self::Line(l) => l.write(to),
            Self::Cmd(l) => l.write(to),
            Self::Math(l) => l.write(to)
        }
    }
}
impl<'a> From<Line<'a>> for Block<'a> {
    fn from(value: Line<'a>) -> Self {
        Self::Line(value)
    }
}
impl<'a> From<BlockCommand<'a>> for Block<'a> {
    fn from(value: BlockCommand<'a>) -> Self {
        Self::Cmd(value)
    }
}
impl<'a> From<MathBlock<'a>> for Block<'a> {
    fn from(value: MathBlock<'a>) -> Self {
        Self::Math(value)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::io::Cursor;

    fn get_write_results<F>(op: F) -> String 
    where F: FnOnce(&mut Cursor<Vec<u8>>) -> () {
        let mut buff = Cursor::new(vec![]);
        op(&mut buff);

        let contents = String::from_utf8(buff.into_inner()).unwrap();
        contents
    }

    #[test]
    fn test_str_ligature() {
        let lhs = ("hello", String::from("goodbye"));
        let rhs = (StrLigature::from(lhs.0), StrLigature::from(lhs.1.clone()));

        assert_eq!(&rhs.0, &StrLigature::Ref(lhs.0));
        assert_eq!(&rhs.1, &StrLigature::Own(lhs.1));

        let contents = get_write_results(|buff| {
            rhs.0.write(buff).unwrap();
        });

        assert_eq!(lhs.0, &contents);
    }

    #[test]
    fn test_singular_command_ligature() {
        let cmd = SingularCommandLigature::new("hello");
        assert_eq!(&cmd.name, &StrLigature::Ref("hello"));

        let contents = get_write_results(|buff| {
            cmd.write(buff).unwrap()
        });

        assert_eq!(&contents, "\\hello");
    }
    #[test]
    fn test_command_argument() {
        let a = CommandArgument::Frag(CommandFragment::Str("hello".into()));
        let b = CommandArgument::Vec(
            vec![
                CommandFragment::Str("thing1".into()),
                CommandFragment::Str("thing2".into())
            ]
        );
        let mut c_build = RelationalCommandArgumentBuilder::new();
        c_build.add_relation("thing1", CommandFragment::Num(1.into()))
            .add_relation("thing2", CommandFragment::Num(2.into()));

        let c = c_build.build();

        let a_content = get_write_results(|buff| {
            a.write(buff).unwrap()
        });
        let b_content = get_write_results(|buff| {
            b.write(buff).unwrap()
        });
        let c_content = get_write_results(|buff| {
            c.write(buff).unwrap()
        });

        assert_eq!(&a_content, "hello");
        assert_eq!(&b_content, "thing1,thing2");
        let c_possibilities = [
            "thing1=1,thing2=2",
            "thing2=2,thing1=1"
        ];
        assert!(c_possibilities.iter().any(|x| x == &c_content), "Got content {c_content}");
    }

    #[test]
    fn test_lig_frag_vec() {
        let values = LigatureFragment::Vec(
            vec![
                "the thing is, ".into(),
                NumLigature::from(3).into(),
                " is a very interesting number.".into()
            ]
        );

        let content = get_write_results(|buff| {
            values.write(buff).unwrap();
        });

        assert_eq!(&content, "the thing is, 3 is a very interesting number.");
    }

    #[test]
    fn test_line() {
        // Fragment already tested
        // Command already tested
        // Only have to test blank, comment, and inline math.

        let math = Line::new_math(
            vec![
                LigatureFragment::from("x + y = "),
                LigatureFragment::from(NumLigature::from(3))
            ]
        );

        let math_content = get_write_results(|buff| {
            math.write(buff).unwrap()
        });

        assert_eq!(&math_content, "$x + y = 3$");

        let blank = Line::Blank;
        let blank_content = get_write_results(|buff| {
            blank.write(buff).unwrap()
        });
        assert_eq!(&blank_content, "");

        let comment = Line::new_comment(
            vec![
                LigatureFragment::from("this should be in a comment...")
            ]
        );
        let comment_content = get_write_results(|buff| {
            comment.write(buff).unwrap()
        });

        assert_eq!(&comment_content, "% this should be in a comment...");
    }

    fn command_contents() -> Line<'static> {
        Line::Frag(LigatureFragment::from("Hello my grand friend!"))
    }

    #[test]
    fn test_command() {
        let arg = CommandArgument::Frag(CommandFragment::Str("argument".into()));

        let commands = [
            (
                Command::new("commandToTest", command_contents(), None),
                "\\commandToTest{Hello my grand friend!}"
            ),
            (
                Command::new_builder("commandToTest", |build| { build.with_post_args(arg.clone()) }, command_contents(), None),
                "\\commandToTest{Hello my grand friend!}{argument}"
            ),
            (
                Command::new_builder("commandToTest", |build| { build.with_post_args_bracket(arg.clone()) }, command_contents(), None),
                "\\commandToTest{Hello my grand friend!}[argument]"
            ),
            (
                Command::new_builder("commandToTest", |build| { build.with_post_args(arg.clone()).with_post_args_bracket(arg.clone()) }, command_contents(), None),
                "\\commandToTest{Hello my grand friend!}[argument]{argument}"
            ),
            (
                Command::new_builder("commandToTest", |build| { build.with_post_args(arg.clone()) }, command_contents(), Some(arg.clone())),
                "\\commandToTest[argument]{Hello my grand friend!}{argument}"
            ),
            (
                Command::new_builder("commandToTest", |build| { build.with_post_args_bracket(arg.clone()) }, command_contents(), Some(arg.clone())),
                "\\commandToTest[argument]{Hello my grand friend!}[argument]"
            ),
            (
                Command::new_builder("commandToTest", |build| { build.with_post_args(arg.clone()).with_post_args_bracket(arg.clone()) }, command_contents(), Some(arg.clone())),
                "\\commandToTest[argument]{Hello my grand friend!}[argument]{argument}"
            )
        ];

        for (i, (command, expected)) in commands.iter().enumerate() {
            let contents = get_write_results(|buff| {
                command.write(buff).unwrap()
            });

            assert_eq!(&contents, expected, "At index {i}");
        }
    }

    #[test]
    fn test_block_command() {
        let arg = CommandArgument::Frag(CommandFragment::Str("argument".into()));
        let block = Block::Line(command_contents());
        let content = vec![block.clone(), block];

        let cases = [
            (
                BlockCommand::new("commandToTest", content.clone(), false),
                "\\begin{commandToTest}\n\tHello my grand friend!\n\tHello my grand friend!\n\\end{commandToTest}"
            ),
            (
                BlockCommand::new_builder("commandToTest", |build| { build.with_post_args(arg.clone() ) }, content.clone(), false),
                "\\begin{commandToTest}{argument}\n\tHello my grand friend!\n\tHello my grand friend!\n\\end{commandToTest}"
            ),
            (
                BlockCommand::new_builder("commandToTest", |build| { build.with_post_args(arg.clone()).with_post_args_bracket(arg.clone()) }, content.clone(), false),
                "\\begin{commandToTest}[argument]{argument}\n\tHello my grand friend!\n\tHello my grand friend!\n\\end{commandToTest}"
            ),
            (
                BlockCommand::new_builder("commandToTest", |build| { build.with_post_args(arg.clone() ) }, content.clone(), true),
                "\\begin{commandToTest}{argument}\n\tHello my grand friend!\\\\\n\tHello my grand friend!\n\\end{commandToTest}"
            ),
            (
                BlockCommand::new_builder("commandToTest", |build| { build.with_post_args(arg.clone()).with_post_args_bracket(arg.clone()) }, content.clone(), true),
                "\\begin{commandToTest}[argument]{argument}\n\tHello my grand friend!\\\\\n\tHello my grand friend!\n\\end{commandToTest}"
            )
        ];

        for (i, (command, expected)) in cases.into_iter().enumerate() {
            let content = get_write_results(|buff| { command.write(buff).unwrap(); } );

            assert_eq!(&content, expected, "On case index {i}")
        }
    }

    #[test]
    fn test_math_block() {
        let block = MathBlock::new(
            vec![
                LigatureFragment::from("x + y = "),
                LigatureFragment::from(NumLigature::from(30))
            ]
        );

        let content = get_write_results(|buff| {
            block.write(buff).unwrap()
        });

        assert_eq!(&content, "\\[\n\tx + y = 30\n\\]");
    }
}