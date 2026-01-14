//
//  BudgetEditor.swift
//  Edmund
//
//  Created by Hollan Sellars on 6/28/25.
//

import SwiftUI
import SwiftData

public struct IncomeDivisionEdit : View {
    public init(_ snap: IncomeDivisionSnapshot) {
        self.snapshot = snap;
    }
    
    @Bindable private var snapshot: IncomeDivisionSnapshot;
    
    public var body: some View {
        TabView {
            Tab {
                IncomeDivisionPropertiesEditor(snapshot: snapshot, isSheet: false)
            } label: {
                Text("Properties")
            }
            
            Tab {
                IncomeDevotionsEditor(snapshot: snapshot)
                    .padding()
            } label: {
                Text("Devotions")
            }
        }.padding()
    }
}

#Preview(traits: .sampleData) {
    @Previewable @Query var divisions: [IncomeDivision];
    
    IncomeDivisionEdit(divisions.first!.makeSnapshot())
}
