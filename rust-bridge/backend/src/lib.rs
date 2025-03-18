use std::ffi::{c_uint, c_ushort};

#[repr(C)]
pub struct Passing {
    num: c_uint,
    thing: c_ushort
}

#[unsafe(no_mangle)]
pub extern "C" fn rust_process(data: Passing) -> Passing {
    Passing {
        num: data.num + 1,
        thing: data.thing - 4
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn create_passing() -> Passing {
    Passing {
        num: 0,
        thing: create_number()
    }
}

pub fn create_number() -> c_ushort {
    16
}
