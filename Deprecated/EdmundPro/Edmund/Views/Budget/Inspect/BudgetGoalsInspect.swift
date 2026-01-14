//
//  BudgetGoalsInspect.swift
//  Edmund
//
//  Created by Hollan Sellars on 8/22/25.
//

import SwiftUI

struct LoadedBudgetGoalsInspect<T> : View where T: BudgetGoal {
    var data: [BudgetGoalData<T>];
    var name: LocalizedStringKey;
    @State private var selection: Set<UUID> = .init();
    
    @Environment(\.horizontalSizeClass) private var horizontalSizeClass;
    
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";
    
    var body: some View {
        Table(data, selection: $selection) {
            TableColumn(name) { row in
                ElementDisplayer(value: row.over.association)
            }
            
            TableColumn("Goal") { row in
                Text(row.over.amount, format: .currency(code: currencyCode))
            }
            TableColumn("Period") { row in
                Text(row.over.period.display)
            }
            
            TableColumn("Monthly Goal") { row in
                Text(row.over.monthlyGoal, format: .currency(code: currencyCode))
            }
            
            TableColumn("Progress") { row in
                Text(row.balance, format: .currency(code: currencyCode))
            }
            
            TableColumn("Money Left") { row in
                Text(row.freeToSpend, format: .currency(code: currencyCode))
            }
            
            TableColumn("Over By") { row in
                Text(row.overBy, format: .currency(code: currencyCode))
            }
        }
    }
}

struct BudgetGoalsInspect<T> : View where T: BudgetGoal {
    var over: BudgetMonthInspectManifest;
    var source: KeyPath<BudgetData, [BudgetGoalData<T>]>
    var name: LocalizedStringKey;
    
    @ViewBuilder
    private var loadingView: some View {
        VStack {
            Spacer()
            
            Text("Please wait while Edmund does some math")
                .italic()
            ProgressView()
            
            Spacer()
        }
    }
    
    var body: some View {
        switch over.cache {
            case .loading: loadingView
            case .error(let e): BudgetGoalsErrorView(e: e)
            case .loaded(let d): LoadedBudgetGoalsInspect(data: d[keyPath: source], name: name)
        }
    }
}
