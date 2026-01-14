//
//  InstancesView.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 12/1/25.
//

import SwiftUI
import SwiftData
import WeatherKit

struct InstancesView : View {
    @Query private var instances: [WeatherInstance]
    @State private var selected: WeatherInstance?;
    @Environment(\.modelContext) private var modelContext;
    
    var body: some View {
        List {
            ForEach(instances) { instance in
                VStack(alignment: .leading) {
                    if let location = instance.location {
                        Text(location.title)
                            .font(.headline)
                    }
                    
                    
                }
                .swipeActions(edge: .leading) {
                    Button {
                        selected = instance
                    } label: {
                        Label("Close Look", systemImage: "magnifyingglass")
                    }.tint(.accentColor)
                }
            }.onDelete { indexes in
                for index in indexes {
                    modelContext.delete(instances[index])
                }
            }
        }.sheet(item: $selected) { item in
            VStack(alignment: .leading) {
                HStack {
                    Text("Weather at \(item.location?.title ?? "(No Location)")")
                        .font(.title2)
                        .bold()
                    Spacer()
                }
                
                List {
                    HStack {
                        Text("Date")
                        Spacer()
                        Text(item.date.formatted(date: .complete, time: .omitted))
                    }
                    
                    HStack {
                        Text("Temperature")
                        Spacer()
                        Text(item.temperature, format: .number.precision(.fractionLength(1)))
                        Text("°C")
                    }
                    
                    HStack {
                        Text("Humidity")
                        Spacer()
                        Text(item.humidity, format: .percent)
                    }
                    
                    HStack {
                        Text("Pressure")
                        Spacer()
                        Text(item.pressure, format: .number.precision(.fractionLength(0)))
                        Text("mmHg")
                    }
                    
                    HStack {
                        Text("Cloud Cover")
                        Spacer()
                        Text(item.cloudCover, format: .percent)
                    }
                    
                    HStack {
                        Text("UV Index")
                        Spacer()
                        Text(item.uvIndex, format: .number.precision(.fractionLength(0)))
                    }
                    
                    HStack {
                        Text("Is Daylight?")
                        Spacer()
                        Text(item.isDaylight ? "Yes" : "No")
                    }
                }
            }.padding()
        }.navigationTitle("Weather Instances")
            .toolbar {
                ToolbarItem(placement: .primaryAction) {
                    EditButton()
                }
            }
    }
}

@MainActor
func prepareContainer() -> ModelContainer {
    let container = try! ModelContainer(for: WeatherInstance.self, LocationInstance.self, configurations: ModelConfiguration(isStoredInMemoryOnly: true))
    let location = LocationInstance(lat: 28.04213, long: -81.95410, title: "Lakeland, FL, USA", subTitle: nil);
    container.mainContext.insert(location);
    let instance = WeatherInstance(temperature: 30, humidity: 0.5, pressure: 764, cloudCover: 0.4, condition: .clear, date: .now, isDaylight: true, uvIndex: 3);
    location.weatherInstances.append(instance);
    container.mainContext.insert(instance);
    
    return container;
}

#Preview {
    NavigationStack {
        InstancesView()
    }.modelContainer(prepareContainer())
}
