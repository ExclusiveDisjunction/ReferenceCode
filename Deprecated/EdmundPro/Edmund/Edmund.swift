//
//  Edmund.swift
//  Edmund
//
//  Created by Hollan on 11/3/24.
//

import SwiftUI
import SwiftData

public let bugFormLink: URL = URL(string: "https://docs.google.com/forms/d/e/1FAIpQLSc4KedjEgIuSnzqhHv6onfxKZZtLlnj3d5kXLJGaOFu70a9Yg/viewform?usp=header")!
public let featureFormLink: URL = URL(string: "https://docs.google.com/forms/d/e/1FAIpQLSewKses93mZERpF5LTmZwnEMhRyyS8p1XQ4_yUnYfgDpuEjhg/viewform?usp=sharing&ouid=107738640373490198091")!

@main
struct EdmundApp: App {
    init() {
        let log = LoggerSystem();
        self.help = HelpEngine(log.help);
        self.log = log
        
        let help = self.help; //Stops from task closure capture
        
        Task {
            await help.walkDirectory()
        }
    }
    
    let log: LoggerSystem;
    let help: HelpEngine;
    
    @AppStorage("themeMode") private var themeMode: ThemeMode?;
    
    private var colorScheme: ColorScheme? {
        switch themeMode {
            case .light: return .light
            case .dark: return .dark
            default: return nil
        }
    }
    
    
    var body: some Scene {
        DocumentGroup(editing: .edmundContainer, migrationPlan: MigrationPlan.self) {
            MainView()
                .environment(\.helpEngine, help)
                .environment(\.loggerSystem, log)
                .preferredColorScheme(colorScheme)
        }.commands {
            GeneralCommands()
        }
        
        Settings {
            SettingsView()
                .environment(\.helpEngine, help)
                .preferredColorScheme(colorScheme)
        }

        /*
        
        WindowGroup("Help", id: "help") {
            HelpTreePresenter()
                .environment(\.helpEngine, help)
                .preferredColorScheme(colorScheme)
        }
         */
    }
}
