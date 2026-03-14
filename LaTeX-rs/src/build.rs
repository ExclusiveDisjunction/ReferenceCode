use crate::lig::{Block, LatexElement, LigatureFragment, Line, NumLigature, StrLigature};


pub trait LatexBuildable<Output> where Output: LatexElement {
    fn build(self) -> Output;
}

impl<'a> LatexBuildable<StrLigature<'a>> for &'a str {
    fn build(self) -> StrLigature<'a> {
        StrLigature::Ref(self)
    }
}
impl LatexBuildable<StrLigature<'static>> for String {
    fn build(self) -> StrLigature<'static> {
        StrLigature::Own(self)
    }
}
impl<'a> LatexBuildable<LigatureFragment<'a>> for &'a str {
    fn build(self) -> LigatureFragment<'a> {
        LigatureFragment::Str(StrLigature::Ref(self))
    }
}
impl LatexBuildable<LigatureFragment<'static>> for String {
    fn build(self) -> LigatureFragment<'static> {
        LigatureFragment::Str(StrLigature::Own(self))
    }
}
impl<'a> LatexBuildable<Line<'a>> for &'a str {
    fn build(self) -> Line<'a> {
        Line::Frag(LigatureFragment::Str(StrLigature::Ref(self)))
    }
}
impl LatexBuildable<Line<'static>> for String {
    fn build(self) -> Line<'static> {
        Line::Frag(LigatureFragment::Str(StrLigature::Own(self)))
    }
}
impl<'a> LatexBuildable<Block<'a>> for &'a str {
    fn build(self) -> Block<'a> {
        Block::Line(Line::Frag(LigatureFragment::Str(StrLigature::Ref(self))))
    }
}
impl LatexBuildable<Block<'static>> for String {
    fn build(self) -> Block<'static> {
        Block::Line(Line::Frag(LigatureFragment::Str(StrLigature::Own(self))))
    }
}

macro_rules! impl_num_ligature {
    ( $($src:ty => $variant:ident ),* $(,)? ) => {
        $(
            impl LatexBuildable<NumLigature> for $src {
                fn build(self) -> NumLigature {
                    NumLigature::$variant(self)
                }
            }

            impl LatexBuildable<LigatureFragment<'static>> for $src {
                fn build(self) -> LigatureFragment<'static> {
                    LigatureFragment::Num(NumLigature::$variant(self))
                }
            }

            impl LatexBuildable<Line<'static>> for $src {
                fn build(self) -> Line<'static> {
                    Line::Frag(LigatureFragment::Num(NumLigature::$variant(self)))
                }
            }
            impl LatexBuildable<Block<'static>> for $src {
                fn build(self) -> Block<'static> {
                    Block::Line(Line::Frag(LigatureFragment::Num(NumLigature::$variant(self))))
                }
            }
        )*
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



pub enum SpecialLigature {
    Backslash,
    Ampersand,
    Percent,
    Less,
    Greater,
    Dollar,
    LBrace,
    RBrace,
    Bar,
    Dash,
    TradeMark,
    Pound,
    Section,
    EmDash,
    Registered,
    Copyright
}

#[cfg(test)]
mod test {
    use super::*;

    fn take_in_builder<T, O>(input: T) -> O where T: LatexBuildable<O>, O: LatexElement {
        input.build()
    }

    #[test]
    fn test_types_one() {
        let _: StrLigature<'_> = take_in_builder("hello!");
        let _: LigatureFragment<'_> = take_in_builder("hello!");
        let _: Line<'_> = take_in_builder("hello!");
        let _: Block<'_> = take_in_builder("hello!");

        let _: NumLigature = take_in_builder(3);
        let _: LigatureFragment<'_> = take_in_builder(3);
        let _: Line<'_> = take_in_builder(3);
        let _: Block<'_> = take_in_builder(3);

    }
}