//
//  ManyManyTransfer.swift
//  Edmund
//
//  Created by Hollan on 12/28/24.
//

import SwiftUI;

struct ManyManyTransfer : TransactionEditorProtocol {
    private var top: ManyTableManifest = .init(isSource: true)
    private var bottom: ManyTableManifest = .init(isSource: false)
    @State private var date: Date = .now;
    
    @Environment(\.modelContext) private var modelContext;
    @Environment(\.categoriesContext) private var categoriesContext;
    
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";

    func apply() -> ValidationFailure? {
        guard let categories = categoriesContext else {
            return .internalError
        }
    
        let topAmount = top.amount, bottomAmount = bottom.amount;
        guard topAmount == bottomAmount else {
            return .invalidInput
        }
        
        let newTrans: [LedgerEntry];
        do {
            let topData: [LedgerEntry] = try top.createTransactions(date: date, cats: categories);
            let bottomData: [LedgerEntry] = try bottom.createTransactions(date: date, cats: categories)
            
            newTrans = topData + bottomData;
        }
        catch let e {
            return e
        }
        
        for transaction in newTrans {
            modelContext.insert(transaction);
        }
        
        return nil;
    }
    
    @ViewBuilder
    private var stats: some View {
        HStack {
            Text("Transfer Out:")
            Text(top.amount, format: .currency(code: currencyCode))
        }
        
       
    }
    
    var body : some View {
        TransactionEditorFrame(.transfer(.manyMany), apply: apply) {
            VStack {
                HStack {
                    Text("Date:")
                    
                    DatePicker("", selection: $date, displayedComponents: .date)
                        .labelsHidden()
                    
                    Button("Today") {
                        date = .now
                    }
                    
                    Spacer()
                }
                
                HStack {
                    Text("Transfer Out:")
                    Text(top.amount, format: .currency(code: currencyCode))
                    
                    Spacer()
                }
                
                HStack {
                    Text("Transfer In:")
                    Text(bottom.amount, format: .currency(code: currencyCode))
                    
                    Spacer()
                }
                
                
                TabView {
                    Tab("Take from", systemImage: "arrow.up.right") {
                        ManyTransferTable(title: nil, data: top)
                            .frame(minHeight: 150, idealHeight: 350)
                    }
                    
                    Tab("Move To", systemImage: "arrow.down.right") {
                        ManyTransferTable(title: nil, data: bottom)
                            .frame(minHeight: 150, idealHeight: 350)
                    }
                }
            }
        }
    }
}

#Preview(traits: .sampleData) {
    ManyManyTransfer()
}
