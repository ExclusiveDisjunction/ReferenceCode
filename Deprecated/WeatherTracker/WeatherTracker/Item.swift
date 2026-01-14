//
//  Item.swift
//  WeatherTracker
//
//  Created by Hollan Sellars on 11/29/25.
//

import Foundation
import SwiftData
import WeatherKit
import CoreLocation

@Model
public final class LocationInstance : Identifiable {
    public init(lat: CLLocationDegrees, long: CLLocationDegrees, title: String, subTitle: String?) {
        self.lat = lat
        self.long = long
        self.title = title
        self.subTitle = subTitle
    }
    
    public var id: UUID = UUID();
    public var lat: CLLocationDegrees;
    public var long: CLLocationDegrees;
    public var title: String;
    public var subTitle: String?;
    
    @Relationship(deleteRule: .cascade, inverse: \WeatherInstance.location)
    public var weatherInstances: [WeatherInstance] = [];
}

@Model
public final class WeatherInstance : Identifiable {
    public init(temperature: Double, humidity: Double, pressure: Double, cloudCover: Double, condition: WeatherCondition, date: Date, isDaylight: Bool, uvIndex: Int) {
        self.temperature = temperature
        self.humidity = humidity
        self.pressure = pressure
        self.cloudCover = cloudCover
        self.condition = condition
        self.date = date
        self.isDaylight = isDaylight
        self.uvIndex = uvIndex
    }
    public convenience init(from: CurrentWeather) {
        self.init(
            temperature: from.temperature.converted(to: UnitTemperature.celsius).value,
            humidity: from.humidity,
            pressure: from.pressure.converted(to: UnitPressure.millimetersOfMercury).value,
            cloudCover: from.cloudCover,
            condition: from.condition,
            date: from.date,
            isDaylight: from.isDaylight,
            uvIndex: from.uvIndex.value
        )
    }
    
    public var id: UUID = UUID();
    public var temperature: Double; // Deg C
    public var humidity: Double;
    public var pressure: Double; // mmHg
    public var cloudCover: Double;
    public var condition: WeatherCondition;
    public var date: Date;
    public var isDaylight: Bool;
    public var uvIndex: Int;
    
    @Relationship(deleteRule: .nullify)
    public var location: LocationInstance? = nil;
}
