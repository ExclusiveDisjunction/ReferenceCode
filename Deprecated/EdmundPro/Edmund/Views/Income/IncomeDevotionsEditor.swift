//
//  BudgetDevotionsEditor.swift
//  Edmund
//
//  Created by Hollan Sellars on 6/28/25.
//

import SwiftUI
import SwiftData

struct IncomeDevotionsEditor : View {
    @Bindable var snapshot: IncomeDivisionSnapshot;
    @State private var selection: Set<IncomeDevotionSnapshot.ID> = .init();
    
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";
    
    @ViewBuilder
    private func amountFor(_ dev: IncomeDevotionSnapshot) -> some View {
        switch dev.amount {
            case .amount(let a): Text(a.rawValue, format: .currency(code: currencyCode))
            case .percent(let p): Text(p.rawValue * snapshot.amount.rawValue, format: .currency(code: currencyCode))
            default: Text("internalError")
        }
    }
    
    private func removeSelected(_ selection: Set<IncomeDevotionSnapshot.ID>? = nil) {
        let trueSelection = selection == nil ? self.selection : selection!;
        
        withAnimation {
            snapshot.devotions.removeAll(where: { trueSelection.contains($0.id) } )
        }
    }
    
    @ViewBuilder
    private var headerRow : some View {
        Text("Income:")
        Text(snapshot.amount.rawValue, format: .currency(code: currencyCode))
        
        Text("Remainder Amount:")
        Text(snapshot.remainderValue, format: .currency(code: currencyCode))
        
        Text("Amount Free:")
            .bold()
        Text(snapshot.moneyLeft, format: .currency(code: currencyCode))
            .bold()
        
        Spacer()
        
        Button {
            withAnimation {
                snapshot.devotions.append(IncomeDevotionSnapshot(withAmount: 0))
            }
        } label: {
            Image(systemName: "dollarsign")
        }.buttonStyle(.borderless)
        
        Button {
            withAnimation {
                snapshot.devotions.append(IncomeDevotionSnapshot(withPercent: 0))
            }
        } label: {
            Image(systemName: "percent")
        }.buttonStyle(.borderless)
        
        Button {
            withAnimation {
                snapshot.devotions.append(IncomeDevotionSnapshot.newRemainder())
            }
        } label: {
            Image(systemName: "add")
        }.buttonStyle(.borderless)
        
        Button {
            removeSelected()
        } label: {
            Image(systemName: "trash")
                .foregroundStyle(.red)
        }.buttonStyle(.borderless)
    }
    
    var body: some View {
        VStack {
            HStack {
                headerRow
            }
            
            Table($snapshot.devotions, selection: $selection) {
                TableColumn("Name") { $dev in
                    TextField("Name", text: $dev.name)
                        .textFieldStyle(.roundedBorder)
                }
                
                TableColumn("Devotion") { $dev in
                    switch dev.amount {
                        case .amount(let a): CurrencyField(a)
                        case .percent(let p): PercentField(p)
                        default: EmptyView()
                    }
                }
                
                TableColumn("Amount") { $dev in
                    amountFor(dev)
                }
                
                TableColumn("Group") { $dev in
                    EnumPicker(value: $dev.group)
                }
                
                TableColumn("Destination") { $dev in
                    ElementPicker($dev.account)
                }
                .width(270)
            }.contextMenu(forSelectionType: IncomeDevotionSnapshot.ID.self) { selection in
                Button {
                    removeSelected(selection)
                } label: {
                    Label("Remove", systemImage: "trash")
                        .foregroundStyle(.red)
                }.disabled(selection.isEmpty)
            }
        }
    }
}

#Preview(traits: .sampleData) {
    @Previewable @Query var divisions: [IncomeDivision];
    
    IncomeDevotionsEditor(snapshot: divisions.first!.makeSnapshot())
        .padding()
}
