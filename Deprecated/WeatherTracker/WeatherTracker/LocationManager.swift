//
//  LocationManager.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 12/1/25.
//

import SwiftUI
import SwiftData

struct LocationAdder : View {
    @StateObject private var cityVm = CitySearchVM();
    @State private var error: Bool = false;
    @Environment(\.modelContext) private var modelContext;
    @Environment(\.dismiss) private var dismiss;
    
    func add() {
        guard let city = cityVm.selected else {
            error = true;
            return;
        }
        
        let location = LocationInstance(lat: city.lat, long: city.long, title: city.title, subTitle: city.subTitle);
        modelContext.insert(location)
        dismiss();
    }

    var body: some View {
        CityPicker(vm: cityVm)
            .toolbar {
                ToolbarItem(placement: .primaryAction) {
                    Button("Done", action: add)
                }
            }
            .alert("Missing City", isPresented: $error) {
                Button("Ok") {
                    error = false;
                }
            } message: {
                Text("Please select a city to add.")
            }
    }
}

struct LocationManager : View {
    @State private var selected: Set<UUID> = .init();
    @Query private var locations: [LocationInstance];
    @Environment(\.modelContext) private var modelContext;
    
    func deleteSelected() {
        withAnimation {
            for item in locations {
                if selected.contains(item.id) {
                    modelContext.delete(item)
                }
            }
        }
    }
    
    var body: some View {
        List {
            ForEach(locations) { location in
                LocationDisplayer(data: location)
            }.onDelete { index in
                withAnimation {
                    for i in index {
                        modelContext.delete(locations[i])
                    }
                }
            }
        }.toolbar {
            ToolbarItem(placement: .primaryAction) {
                NavigationLink {
                    LocationAdder()
                } label: {
                    Label("Add", systemImage: "plus")
                }
                
                Button {
                    deleteSelected()
                } label: {
                    Label("Delete", systemImage: "trash")
                        .foregroundStyle(.red)
                }
            }
            
            ToolbarItem(placement: .primaryAction) {
                EditButton()
            }
        }.navigationTitle("Locations")
    }
}

#Preview {
    NavigationStack {
        LocationManager()
    }
    .modelContainer(for: [LocationInstance.self, WeatherInstance.self], inMemory: true)
}
