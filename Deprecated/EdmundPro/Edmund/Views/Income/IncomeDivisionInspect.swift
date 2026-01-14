//
//  BudgetInspect.swift
//  Edmund
//
//  Created by Hollan Sellars on 6/28/25.
//

import SwiftUI
import SwiftData

public struct IncomeDivisionInspect : View {
    public var data: IncomeDivision
    
    public var body: some View {
        TabView {
            
            Tab {
                VStack {
                    IncomeDivisionPropertiesInspect(data: data, isSheet: false)
                    Spacer()
                }
            } label: {
                Text("Properties")
            }
            
            Tab {
                IncomeDevotionsInspect(data: data)
            } label: {
                Text("Devotions")
            }
        }.padding()
    }
}

#Preview(traits: .sampleData){
    @Previewable @Query var divisions: [IncomeDivision];
    
    IncomeDivisionInspect(data: divisions.first!)
        .padding()
}
