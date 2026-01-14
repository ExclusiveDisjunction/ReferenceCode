//
//  BudgetIncomeInspect.swift
//  Edmund
//
//  Created by Hollan Sellars on 8/22/25.
//

import SwiftUI
import SwiftData

struct BudgetMonthIncomeInspect : View {
    var over: BudgetMonthInspectManifest
    @State private var selection: Set<IncomeDivision.ID> = .init();
    
    @Environment(\.horizontalSizeClass) private var horizontalSizeClass;
    
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";
    
    var body: some View {
        Table(over.over.income, selection: $selection) {
            TableColumn("Name", value: \.name)
            TableColumn("Amount") { income in
                Text(income.amount, format: .currency(code: currencyCode))
            }
            
            TableColumn("Income Kind") { income in
                EnumDisplayer(value: income.kind)
            }
            
            TableColumn("Deposit To") { income in
                ElementDisplayer(value: income.depositTo)
            }
            
            TableColumn("") { income in
                if income.isFinalized {
                    Image(systemName: "checkmark")
                }
            }.width(30)
        }
    }
}
