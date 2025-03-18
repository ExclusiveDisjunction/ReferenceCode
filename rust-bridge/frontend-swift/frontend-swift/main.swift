//
//  main.swift
//  frontend-swift
//
//  Created by Hollan on 3/17/25.
//

import Foundation

print("Hello, World!")

let passing = create_passing();
let revised = rust_process(passing);

print(revised.num);
