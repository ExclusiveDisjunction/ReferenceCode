//
//  BudgetDevotionsInspect.swift
//  Edmund
//
//  Created by Hollan Sellars on 6/28/25.
//

import SwiftUI
import SwiftData

struct IncomeDevotionsInspect : View {
    var data: IncomeDivision
    @State private var selection: Set<IncomeDevotion.ID> = .init();
    
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";
    
    private func computedAmount(_ target: IncomeDevotion) -> Decimal {
        switch target.amount {
            case .amount(let a): a
            case .percent(let p): p * data.amount
            case .remainder: data.remainderValue
        }
    }
    
    @ViewBuilder
    private var fullSize: some View {
        Table(data.devotions, selection: $selection) {
            TableColumn("Name", value: \.name)
            TableColumn("Devotion") { row in
                switch row.amount {
                    case .amount(let a): Text(a, format: .currency(code: currencyCode))
                    case .percent(let p): Text(p, format: .percent)
                    case .remainder: Text("Remainder")
                }
            }
            TableColumn("Amount") { row in
                Text(computedAmount(row), format: .currency(code: currencyCode))
            }
            TableColumn("Group") { row in
                Text(row.group.display)
            }
            TableColumn("Destination") { row in
                ElementDisplayer(value: row.account)
            }
            .width(150)
        }
    }
    
    var body: some View {
        VStack {
            HStack {
                Text("Total Income:")
                Text(data.amount, format: .currency(code: currencyCode))
                
                Spacer()
                
                Text("Amount Free:", comment: "This in context is the amount of money left from the income of the divider, minus all devotions. This is similar to variance.")
                Text(data.variance, format: .currency(code: currencyCode))
            }
            
            fullSize
        }.padding()
    }
}

#Preview(traits: .sampleData) {
    @Previewable @Query var divisions: [IncomeDivision];
    
    IncomeDevotionsInspect(data: divisions.first!)
}
