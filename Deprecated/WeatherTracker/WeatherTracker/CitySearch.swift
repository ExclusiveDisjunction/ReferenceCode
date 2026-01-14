//
//  CitySearch.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 11/30/25.
//

import SwiftUI
import MapKit
import Combine

protocol LocationEncoder {
    var lat: CLLocationDegrees { get }
    var long: CLLocationDegrees { get }
    var title: String { get }
    var subTitle: String? { get }
}

extension LocationInstance : LocationEncoder { }

struct SearchLocation : LocationEncoder {
    var lat: CLLocationDegrees;
    var long: CLLocationDegrees;
    var title: String;
    var subTitle: String?;
}

struct LocationDisplayer<T> : View where T: LocationEncoder {
    let data: T;
    
    var body: some View {
        VStack(alignment: .leading) {
            Text(data.title).font(.headline)
            if let subTitle = data.subTitle {
                Text(subTitle).font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
    }
}

@MainActor
class CitySearchVM: NSObject, ObservableObject, MKLocalSearchCompleterDelegate {
    @Published var query = "";
    @Published var suggestions: [MKLocalSearchCompletion] = [];
    @Published var selected: SearchLocation? = nil;
    
    private let completer = MKLocalSearchCompleter()
    
    override init() {
        super.init()
        completer.delegate = self
    }
    
    func updateSearch() {
        completer.queryFragment = query;
    }
    
    func completerDidUpdateResults(_ completer: MKLocalSearchCompleter) {
        suggestions = completer.results
    }
    
    func selectCity(_ suggestion: MKLocalSearchCompletion) async {
        let request = MKLocalSearch.Request(completion: suggestion)
        let response = try? await MKLocalSearch(request: request).start()
        
        if let coord = response?.mapItems.first?.placemark.coordinate {
            self.selected = SearchLocation(
                lat: coord.latitude,
                long: coord.longitude,
                title: suggestion.title,
                subTitle: suggestion.subtitle
            )
        }
    }
}

struct CityPicker : View {
    @StateObject var vm: CitySearchVM;
    
    @ViewBuilder
    func displayCompletion(_ from: MKLocalSearchCompletion) -> some View {
        VStack(alignment: .leading) {
            Text(from.title).font(.headline)
            Text(from.subtitle).font(.caption)
                .foregroundStyle(.secondary)
        }
    }
    
    var body: some View {
        VStack {
            TextField("City Name", text: $vm.query)
                .padding()
                .onChange(of: vm.query) { _, _ in vm.updateSearch() }
                .textFieldStyle(.roundedBorder)
            
            List {
                if let selected = vm.selected {
                    HStack {
                        Image(systemName: "checkmark")
                            .tint(.accentColor)
                        
                        LocationDisplayer(data: selected)
                    }
                }
                
                ForEach(vm.suggestions, id: \.self) { suggestion in
                    Button {
                        Task {
                            await vm.selectCity(suggestion)
                        }
                    } label: {
                        displayCompletion(suggestion)
                    }
                }
            }
        }
    }
}

#Preview {
    @Previewable @StateObject var vm = CitySearchVM();
    
    CityPicker(vm: vm)
}
