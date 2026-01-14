//
//  WeatherTrackerApp.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 11/29/25.
//

import SwiftUI
import SwiftData
import BackgroundTasks

@main
struct WeatherTrackerApp: App {
    init() {
        let result = BGTaskScheduler.shared.register(forTaskWithIdentifier: backgroundThreadIdentifier, using: nil) { task in
            print("Task invoked")
            handleBkWork(task: task as! BGProcessingTask)
        }
        
        if !result {
            print("Thread request for background processing failed.")
        }
        else {
            print("Thread request for background processing succeded.")
        }
    }
    
    @State private var scheduledBk = false;
    
    var body: some Scene {
        WindowGroup {
            ContentView()
                .task {
                    print("Scheduling work")
                    scheduleBkWork()
                    print("Work scheduled")
                }
        }.modelContainer(for: [WeatherInstance.self, LocationInstance.self], isAutosaveEnabled: true, isUndoEnabled: true)
    }
}
