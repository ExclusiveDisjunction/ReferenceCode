//
//  WeatherTrackerApp.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 11/29/25.
//

import SwiftUI
import SwiftData
import BackgroundTasks
import os

class AppDelegate : NSObject, UIApplicationDelegate {
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        let result = BGTaskScheduler.shared.register(forTaskWithIdentifier: backgroundThreadIdentifier, using: nil) { task in
            Logger(subsystem: "com.exdisj.WeatherTracker", category: "background").info("Task invoked")
            handleBkWork(task: task as! BGAppRefreshTask)
            task.setTaskCompleted(success: true)
        }
        
        if !result {
            print("Thread request for background processing failed.")
        }
        else {
            print("Thread request for background processing succeded.")
        }
        
        return true
    }
}

@main
struct WeatherTrackerApp: App {
    init() {
        
    }
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate;
    @State private var scheduledBk = false;
    
    var body: some Scene {
        WindowGroup {
            ContentView()
                .task {
                    print("Scheduling work")
                    scheduleBkWork()
                    print("Work scheduled")
                    
                    let _ = try? await Task.sleep(for: .seconds(20));
                    
                    let requests = await BGTaskScheduler.shared.pendingTaskRequests();
                    print("The current pending tasks are: \(String(describing: requests))");
                }
        }.modelContainer(for: [WeatherInstance.self, LocationInstance.self], isAutosaveEnabled: true, isUndoEnabled: true)
    }
}
