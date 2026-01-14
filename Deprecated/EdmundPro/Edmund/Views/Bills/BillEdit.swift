//
//  BillEdit.swift
//  Edmund
//
//  Created by Hollan Sellars on 5/26/25.
//

import SwiftUI
import SwiftData

/// The edit view for Bills.
public struct BillEdit : View {
    @Bindable private var snapshot: BillSnapshot;
    @State private var showingSheet = false;
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";
    
    public init(_ snapshot: BillSnapshot) {
        self.snapshot = snapshot;
    }
    
#if os(macOS)
    private let minWidth: CGFloat = 80;
    private let maxWidth: CGFloat = 90;
#else
    private let minWidth: CGFloat = 110;
    private let maxWidth: CGFloat = 120;
#endif
    
    public var body: some View {
        Grid {
            GridRow {
                Text("Name:").frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                TextField("Name", text: $snapshot.name)
                    .textFieldStyle(.roundedBorder)
            }
            
            Divider()
            
            GridRow {
                Text("Start Date:")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                HStack {
                    DatePicker("", selection: $snapshot.startDate, displayedComponents: .date)
                        .labelsHidden()
                    Spacer()
                }
            }
            
            GridRow {
                Text("Has End Date:").frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                HStack {
                    Toggle("", isOn: $snapshot.hasEndDate).labelsHidden()
                    Spacer()
                }
            }
            
            if snapshot.hasEndDate {
                GridRow {
                    Text("End Date:")
                        .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                    
                    HStack {
                        DatePicker("End", selection: $snapshot.endDate, displayedComponents: .date)
                            .labelsHidden()
                        
                        Spacer()
                    }
                }
            }
            
            Divider()
            
            GridRow {
                Text("Company:")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                TextField("Company", text: $snapshot.company)
                    .textFieldStyle(.roundedBorder)
            }
            
            GridRow {
                Text("Has Location:")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                HStack {
                    Toggle("", isOn: $snapshot.hasLocation).labelsHidden()
                    Spacer()
                }
            }
            
            if snapshot.hasLocation {
                GridRow {
                    Text("Location:")
                        .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                    
                    TextField("Location", text: $snapshot.location)
                        .textFieldStyle(.roundedBorder)
                }
            }
            
            GridRow {
                Text("Autopay:")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                
                HStack {
                    Toggle("", isOn: $snapshot.autoPay)
                        .labelsHidden()
                    Spacer()
                }
            }
            
            Divider()
            
            GridRow {
                Text("Frequency:").frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                HStack {
                    Picker("Frequency", selection: $snapshot.period) {
                        ForEach(TimePeriods.allCases, id: \.id) { period in
                            Text(period.display).tag(period)
                        }
                    }.labelsHidden()
                    Spacer()
                }
            }
            
            Divider()
            
            GridRow {
                Text("")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                HStack {
                    Button(action: { showingSheet = true } ) {
                        Label("Edit History", systemImage: "pencil")
                    }
                    Spacer()
                }
            }.sheet(isPresented: $showingSheet) {
                BillHistoryEdit(snapshot: snapshot)
            }
            
            Divider()
            
            GridRow {
                Text("Amount:")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                if snapshot.kind == .utility {
                    Text(snapshot.averageAmount, format: .currency(code: currencyCode))
                }
                else {
                    CurrencyField(snapshot.amount)
                }
            }
            
            GridRow {
                Text("Kind:")
                    .frame(minWidth: minWidth, maxWidth: maxWidth, alignment: .trailing)
                
                HStack {
                    Picker("", selection: $snapshot.kind) {
                        ForEach(BillsKind.allCases, id: \.id) { kind in
                            Text(kind.display)
                                .tag(kind)
                        }
                    }.labelsHidden()
                        .pickerStyle(.segmented)
                    
                    TooltipButton("Subscriptions can usually be canceled whenever, while bills have stricter requirements.")
                }
            }
        }
    }
}

#Preview(traits: .sampleData) {
    ElementEditor(Bill(kind: .subscription), adding: false)
}
