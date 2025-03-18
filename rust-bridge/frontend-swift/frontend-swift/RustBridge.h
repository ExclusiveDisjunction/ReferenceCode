//
//  RustBridge.h
//  frontend-swift
//
//  Created by Hollan on 3/17/25.
//

#ifndef RustBridge_h
#define RustBridge_h

struct Passing {
    unsigned int num;
    unsigned short thing;
};

struct Passing rust_process(struct Passing data);
struct Passing create_passing();


#endif /* RustBridge_h */
