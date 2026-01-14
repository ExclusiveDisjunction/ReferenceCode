//
//  ContentView.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 11/29/25.
//

import SwiftUI
import SwiftData
import MapKit

struct AppSettings : Codable {
    
}

enum RecordFrequency: Int, Codable, Sendable, CaseIterable, Identifiable {
    case hour
    case twoHour
    case threeHour
    case day
    case twoDay
    
    var id: Self {
        self
    }
    var label: LocalizedStringKey {
        switch self {
        case .hour: "Every Hour"
        case .twoHour: "Every 2 Hours"
        case .threeHour: "Every 3 Hours"
        case .day: "Every Day"
        case .twoDay: "Every Two Days"
        }
    }
}

struct SettingsView : View {
    @AppStorage("Frequency") var frequency: RecordFrequency = .hour;
    @AppStorage("enabled") var collectionEnabled: Bool = true;
    @AppStorage("cityLat") var savedLat: CLLocationDegrees = 0;
    @AppStorage("cityLong") var savedLong: CLLocationDegrees = 0;
    
    var mainPage: some View {
        Form {
            Toggle("Enable Collection", isOn: $collectionEnabled)
            
            Picker("Record Frequency", selection: $frequency) {
                ForEach(RecordFrequency.allCases) { frequency in
                    Text(frequency.label).tag(frequency)
                }
            }.disabled(!collectionEnabled)
            
            NavigationLink("Location") {
                LocationManager()
            }
        }
    }
    
    var body: some View {
        NavigationStack {
            mainPage
        }
    }
}

enum Pages : Int, Identifiable, CaseIterable {
    case settings
    case instances
    
    var id: Self {
        self
    }
    var display: LocalizedStringKey {
        switch self {
        case .settings: "Settings"
        case .instances: "Instances"
        }
    }
}

struct ContentView: View {
    @State private var selectedPage: Pages? = .settings;
    
    var body: some View {
        NavigationSplitView {
            List(selection: $selectedPage) {
                ForEach(Pages.allCases) { page in
                    Text(page.display).tag(page)
                }
            }
        } detail: {
            switch (selectedPage ?? .settings) {
            case .settings: SettingsView().navigationTitle("Settings")
            case .instances: InstancesView().navigationTitle("Instances")
            }
        }
    }
}

#Preview {
    ContentView()
        .modelContainer(for: WeatherInstance.self, inMemory: true)
}
