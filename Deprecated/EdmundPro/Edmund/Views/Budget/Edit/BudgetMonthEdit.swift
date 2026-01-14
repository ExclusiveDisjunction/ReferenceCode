//
//  BudgetMonthEdit.swift
//  Edmund
//
//  Created by Hollan Sellars on 8/2/25.
//

import SwiftUI
import SwiftData

struct BudgetMonthEdit : View {
    let source: BudgetMonthSnapshot;
    
    var body: some View {
        VStack {
            HStack {
                Text(source.title)
                    .font(.title)
                Spacer()
            }
            
            TabView {
                Tab {
                    BudgetIncomeEdit(snapshot: source)
                } label: {
                    Text("Income")
                }
                
                Tab {
                    BudgetSpendingGoalEdit(snapshot: source)
                } label: {
                    Text("Spending")
                }
                
                Tab {
                    BudgetSavingGoalEdit(snapshot: source)
                } label: {
                    Text("Savings")
                }
            }
        }
    }
}

#Preview(traits: .sampleData) {
    @Previewable @Query var budgets: [BudgetMonth];
    
    BudgetMonthEdit(source: budgets.first!.makeSnapshot())
        .padding()
}
