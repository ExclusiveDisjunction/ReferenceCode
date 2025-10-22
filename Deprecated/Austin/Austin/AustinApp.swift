//
//  AustinApp.swift
//  Austin
//
//  Created by Hollan Sellars on 6/30/25.
//

import SwiftUI

@main
struct AustinApp: App {
    var body: some Scene {
        DocumentGroup(newDocument: AustinDocument(content: .init())) { file in
            MainView(document: file.$document)
        }.commands {
            MainCommands()
        }
    }
}
