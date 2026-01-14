//
//  Auditor.swift
//  Edmund
//
//  Created by Hollan Sellars on 7/22/25.
//

import SwiftUI
import SwiftData

struct Auditor : View {
    @Query private var accounts: [Account];
    @State private var auditRows: [AccountsAuditorRow] = [];
    @State private var creditRows: [CreditCardRow] = [];
    
    private func refresh() {
        let balances = BalanceResolver(accounts).computeBalances();
        
        self.auditRows = balances.map { AccountsAuditorRow(account: $0.key, balance: $0.value.balance) }
        
        self.creditRows = balances.filter { $0.key.creditLimit != nil }
            .map { bal in
                CreditCardRow(
                    account: bal.key,
                    balance: bal.value.balance
                )
            }
    }
    
    var body: some View {
        VStack {
            TabView {
                Tab {
                    AccountsAuditor(rows: $auditRows)
                } label: {
                    Text("Accounts")
                }
                
                Tab {
                    CreditCardHelper(rows: $creditRows)
                } label: {
                    Text("Credit Cards")
                }
            }
        }.padding()
            .navigationTitle("Auditor")
            .onAppear(perform: refresh)
            .toolbar {
                TopicToolbarButton("Auditing.md", placement: .secondaryAction)
                
                ToolbarItem(placement: .primaryAction) {
                    Button(action: refresh) {
                        Label("Refresh", systemImage: "arrow.trianglehead.clockwise")
                    }
                }
            }
    }
}

#Preview(traits: .sampleData) {
    Auditor()
}
