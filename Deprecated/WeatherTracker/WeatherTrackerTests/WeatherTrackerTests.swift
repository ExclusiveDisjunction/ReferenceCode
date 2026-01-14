//
//  WeatherTrackerTests.swift
//  WeatherTrackerTests
//
//  Created by Hollan Sellars on 12/1/25.
//

import Testing
import WeatherTracker
import SwiftData

struct WeatherTrackerTests {

    @Test func backgroundProcessing() async throws {
        let schema = Schema([WeatherInstance.self, LocationInstance.self]);
        let config = ModelConfiguration(schema: schema, isStoredInMemoryOnly: true, allowsSave: true);
        let container = try ModelContainer(for: WeatherInstance.self, LocationInstance.self, configurations: config);
        
        let operation = WeatherRecorder(container: container);
        
        await MainActor.run {
            let location = LocationInstance(lat: 28.04213, long: -81.95410, title: "Lakeland, FL, USA", subTitle: nil);
            container.mainContext.insert(location);
        }
        
        await operation.asyncStart();
        
        try await MainActor.run {
            let fetch = try container.mainContext.fetch(FetchDescriptor<WeatherInstance>());
            
            #expect(fetch.count == 1);
        }
    }

}
