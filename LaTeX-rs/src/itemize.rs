use crate::{build::LatexBuildable, lig::{Block, BlockCommand, Command, LatexElement, LigatureFragment, Line, SingularCommandLigature}};


pub enum BlockOrFrag<'a> {
    Block(Block<'a>),
    Frag(LigatureFragment<'a>)
}

pub struct ItemizeBuilder<'a> {
    items: Vec<BlockOrFrag<'a>>
}
impl<'a> ItemizeBuilder<'a> {
    pub fn add_line<B, O>(&mut self, builder: B) -> &mut Self 
    where B: LatexBuildable<O>,
    O: LatexElement + Into<LigatureFragment<'a>> {
        let item = builder.build().into();
        self.items.push(BlockOrFrag::Frag(item));

        self
    }

    pub fn add_block<B, O>(&mut self, builder: B) -> &mut Self 
    where B: LatexBuildable<O>,
    O: LatexElement + Into<Block<'a>> {
        let item = builder.build().into();
        self.items.push(BlockOrFrag::Block(item));

        self
    }
}
impl<'a> LatexBuildable<Block<'a>> for ItemizeBuilder<'a> {
    fn build(self) -> Block<'a> {
        let mut lines = Vec::new();

        for item in self.items {
            match item {
                BlockOrFrag::Frag(x) => {
                    let new_line = vec![
                        LigatureFragment::from(SingularCommandLigature::new("item")).into(),
                        x
                    ];
                    let line = Line::Frag(
                        LigatureFragment::Vec(new_line)
                    );

                    lines.push(Block::Line(line));
                },
                BlockOrFrag::Block(x) => {
                    let new_block = BlockCommand::new("item", vec![x], false);

                    lines.push(Block::Cmd(new_block));
                }
            }
        }

        let cmd = BlockCommand::new("itemize", lines, false);

        Block::Cmd(cmd)
    }
}

pub struct MathAlignBlock<'a> {
    content: Vec<LigatureFragment<'a>>
}