//
//  AllExpiredBillsVE.swift
//  Edmund
//
//  Created by Hollan on 4/5/25.
//

import SwiftUI
import SwiftData

struct AllExpiredBillsVE : View {
    @Query(
        filter: #Predicate<Bill> { bill in
            true
        },
        sort: \.name
    ) private var bills: [Bill];
    @State private var selection = Set<Bill.ID>();
    @Bindable private var inspect: InspectionManifest<Bill> = .init()
    @Bindable private var deleting: DeletingManifest<Bill> = .init()
    @Bindable private var warning: SelectionWarningManifest = .init()
    
    @Environment(\.modelContext) private var modelContext;
    
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";
   
    @ViewBuilder
    private var empty: some View {
        VStack {
            Spacer()
            Text("There are no expired bills").italic()
            Spacer()
        }
    }
    
    @ToolbarContentBuilder
    public func toolbar() -> some ToolbarContent {
        /*
        ToolbarItem(placement: .secondaryAction) {
            QueryButton(provider: query)
        }
         */
        
        GeneralIEToolbarButton(on: bills, selection: $selection, inspect: inspect, warning: warning, role: .edit, placement: .primaryAction)
        GeneralIEToolbarButton(on: bills, selection: $selection, inspect: inspect, warning: warning, role: .inspect, placement: .primaryAction)
        GeneralDeleteToolbarButton(on: bills, selection: $selection, delete: deleting, warning: warning, placement: .primaryAction)

    }
    
    @ViewBuilder
    private var regular: some View {
        if bills.isEmpty {
            empty
        }
        else {
            Table(bills, selection: $selection) {
                TableColumn("Name", value: \.name)
                TableColumn("Kind") { bill in
                    Text(bill.kind.display)
                }
                TableColumn("Started On") { bill in
                    Text(bill.startDate.formatted(date: .abbreviated, time: .omitted))
                }
                TableColumn("Ended On") { bill in
                    if let end = bill.endDate {
                        Text(end.formatted(date: .abbreviated, time: .omitted))
                    }
                    else {
                        Text("No Information").italic()
                    }
                }
                TableColumn("Amount") { bill in
                    Text(bill.amount, format: .currency(code: currencyCode))
                }
                TableColumn("Frequency") { bill in
                    Text(bill.period.perName)
                }
            }.contextMenu(forSelectionType: Bill.ID.self) { selection in
                SelectionContextMenu(selection, data: bills, inspect: inspect, delete: deleting, warning: warning)
            }
            .frame(minWidth: 350)
        }
    }
    
    var body: some View {
        regular
            .navigationTitle("Expired Bills")
            .toolbar(content: toolbar)
            .sheet(item: $inspect.value) { bill in
                ElementIE(bill, mode: inspect.mode)
            }
            .alert("Warning", isPresented: $warning.isPresented, actions: {
                Button("Ok") {
                    warning.isPresented = false
                }
            }, message: {
                Text((warning.warning ?? .noneSelected).message)
            })
            .confirmationDialog("deleteItemsConfirm", isPresented: $deleting.isDeleting, titleVisibility: .visible) {
                DeletingActionConfirm(deleting)
            }.padding()
    }
}

#Preview(traits: .sampleData) {
    NavigationStack {
        AllExpiredBillsVE()
    }
}
