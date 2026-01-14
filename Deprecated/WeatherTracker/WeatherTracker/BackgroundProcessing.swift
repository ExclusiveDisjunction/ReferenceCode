//
//  BackgroundProcessing.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 12/1/25.
//

import Foundation
import MapKit
@preconcurrency import BackgroundTasks
import WeatherKit
import os
import SwiftData

let backgroundThreadIdentifier = "com.exdisj.WeatherTracker.weatherRecord";


func scheduleBkWork() {
    let logger = Logger(subsystem: "com.exdisj.WeatherTracker", category: "Background");
    logger.info("Scheduling background thread work.");
    
    let request = BGAppRefreshTaskRequest(identifier: backgroundThreadIdentifier)
    request.earliestBeginDate = Date().addingTimeInterval(1 * 60 * 60); //1 HOur
    
    do {
        try BGTaskScheduler.shared.submit(request)
    } catch {
        print("Could not schedule app refresh: \(error)")
    }
    logger.info("Work has been scheduled for \(request.earliestBeginDate ?? .distantPast, privacy: .public)")
    BGTaskScheduler.shared.getPendingTaskRequests {
        logger.debug("The current pending tasks are: \(String(describing: $0), privacy: .public)");
    }
}

public class AsyncOperation : Operation, @unchecked Sendable {
    public override var isAsynchronous: Bool {
        true
    }
    
    public override init() {
        super.init();
    }
    
    private var _executing = false;
    private var _finished = false;
    public override var isExecuting: Bool {
        get { _executing }
        set {
            willChangeValue(forKey: "isExecuting")
            _executing = newValue;
            didChangeValue(forKey: "isExecuting")
        }
    }
    public override var isFinished: Bool {
        get { _finished }
        set {
            willChangeValue(forKey: "isFinished")
            _finished = newValue;
            didChangeValue(forKey: "isFinished")
        }
    }
    
    open func asyncStart() async {
    
    }
    
    public override func start() {
        print("[Async] Preparing task.")
        guard !isCancelled else {
            finish()
            return
        }
        
        isExecuting = true;
        
        print("[Async] Starting execution.")
        Task { [self] in
            print("[Async] Beginning")
            await self.asyncStart();
            print("[Async] complete.");
            self.finish()
        }
    }
    
    private func finish() {
        isExecuting = false;
        isFinished = true;
    }
}

public class WeatherRecorder : AsyncOperation, @unchecked Sendable {
    public override init() {
        self.logger = Logger(subsystem: "com.exdisj.WeatherTracker", category: "Background");
        self.logger.debug("Creating from unowned context.")
        
        let schema = Schema([WeatherInstance.self, LocationInstance.self]);
        let config = ModelConfiguration(schema: schema, isStoredInMemoryOnly: false, allowsSave: true);
        self.container = try? ModelContainer(for: WeatherInstance.self, LocationInstance.self, configurations: config);
        super.init();
    }
    public init(container: ModelContainer?) {
        self.logger = Logger(subsystem: "com.exdisj.WeatherTracker", category: "Background")
        self.logger.debug("Creating from owned context.")
        self.container = container
        
        super.init();
    }
    
    private var logger: Logger;
    private var container: ModelContainer?;
    
    struct FetchedCx {
        let data: [LocationInstance];
    }
    struct PreparedCx {
        let weather: CurrentWeather;
        let from: LocationInstance;
    }
    
    public override func asyncStart() async {
        guard let container = self.container else {
            self.logger.error("Unable to obtain the model container.");
            return;
        }
        
        let logger = self.logger;
        let cx = ModelContext(container);
        logger.info("Fetching locations.");
        
        let locations: [LocationInstance];
        do {
            locations = try cx.fetch(FetchDescriptor<LocationInstance>());
        }
        catch let e {
            logger.error("Unable to fetch locations: \(e.localizedDescription)");
            return;
        }
        
        guard !locations.isEmpty else {
            logger.warning("No work is to be done. Exiting task.");
            return;
        }
        
        logger.info("Obtained \(locations.count) location(s). Resolving weather.");
        let weather = WeatherService.shared;
        
        var results: [PreparedCx] = [];
        for location in locations {
            let result: CurrentWeather;
            do {
                result = try await weather.weather(for: CLLocation(latitude: location.lat, longitude: location.long)).currentWeather
            }
            catch let e {
                logger.warning("Unable to fetch weather for (\(location.lat) lat, \(location.long) long), due to error '\(e.localizedDescription)'");
                continue;
            }
            
            results.append(PreparedCx(weather: result, from: location))
        }
        
        logger.info("Processed \(results.count) result(s), inserting into model context.")
        
        for result in results {
            let weather = WeatherInstance(from: result.weather);
            result.from.weatherInstances.append(weather);
            cx.insert(weather)
        }
            
        logger.info("Models inserting. Saving.")
        
        do {
            try cx.save()
        }
        catch let e {
            logger.error("Unable to save container \(e.localizedDescription)")
        }
        
        logger.info("Background processing complete.")
    }
}

func handleBkWork(task: BGAppRefreshTask) {
    // Schedule a new refresh task.
    scheduleBkWork()
    
    let logger = Logger(subsystem: "Background Manager", category: "Weather Tracker")
    
    logger.info("Creating Queue")
    let queue = OperationQueue()
    queue.maxConcurrentOperationCount = 1
    
    logger.info("Creating recorder.")
    // Create an operation that performs the main part of the background task.
    let operation = WeatherRecorder()
    
    // Provide the background task with an expiration handler that cancels the operation.
    task.expirationHandler = {
        operation.cancel()
        task.setTaskCompleted(success: false);
    }
    
    
    // Inform the system that the background task is complete
    // when the operation completes.
    operation.completionBlock = {
        queue.cancelAllOperations()
        task.setTaskCompleted(success: !operation.isCancelled);
    }
    
    logger.info("Adding Operation")
    // Start the operation.
    queue.addOperation(operation)
}
