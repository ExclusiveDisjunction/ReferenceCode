//
//  AllBillsEditor.swift
//  Edmund
//
//  Created by Hollan Sellars on 3/26/25.
//

import SwiftUI
import SwiftData
import Charts

struct AllBillsViewEdit : View {
    @Query(
        filter: #Predicate<Bill> { bill in
            true
        },
        sort: \.name
    ) private var bills: [Bill];
    @State private var selection = Set<Bill.ID>();
    @State private var showingChart: Bool = false;
    
    @Bindable private var inspect: InspectionManifest<Bill> = .init();
    @Bindable private var deleting: DeletingManifest<Bill> = .init();
    @Bindable private var warning = SelectionWarningManifest()
    
    @Environment(\.openWindow) private var openWindow;
    @Environment(\.uniqueEngine) private var uniqueEngine;
    @Environment(\.modelContext) var modelContext;
    
    @AppStorage("showcasePeriod") private var showcasePeriod: TimePeriods = .weekly;
    @AppStorage("currencyCode") private var currencyCode: String = Locale.current.currency?.identifier ?? "USD";

    private func addBill(_ kind: BillsKind) {
        let raw = Bill(name: "", kind: kind, amount: 0, company: "", location: nil, start: Date.now, end: nil, period: .monthly)
        withAnimation {
            inspect.open(raw, mode: .add)
        }
    }
    
    private var totalPPP: Decimal {
        bills.reduce(0.0) {
            $0 + ($1.isExpired ? 0 : $1.pricePer(showcasePeriod))
        }
    }
    
    @ViewBuilder
    private var wide: some View {
        Table(bills, selection: $selection) {
            TableColumn("Name", value: \.name)
            TableColumn("Kind") { bill in
                Text(bill.kind.display)
            }
            TableColumn("Amount") { bill in
                Text(bill.amount, format: .currency(code: currencyCode))
            }
            TableColumn("Frequency") { bill in
                Text(bill.period.perName)
            }
            TableColumn("Next Due Date") { bill in
                Text((bill.nextDueDate?.formatted(date: .abbreviated, time: .omitted) ?? "-"))
            }
            
            TableColumn("Set-Aside Cost") { bill in
                let price = bill.isExpired ? 0.0 : bill.pricePer(showcasePeriod);
                Text(price, format: .currency(code: currencyCode))
            }
        }.contextMenu(forSelectionType: Bill.ID.self) { selection in
            SelectionContextMenu(selection, data: bills, inspect: inspect, delete: deleting, warning: warning)
        }
        .frame(minWidth: 320)
    }
    
    @ToolbarContentBuilder
    private var toolbar: some CustomizableToolbarContent {
        /*
        ToolbarItem(id: "query", placement: .secondaryAction) {
            QueryButton(provider: query)
        }
         */
        
        ToolbarItem(id: "graph", placement: .secondaryAction) {
            Button {
                showingChart = true
            } label: {
                Label(showingChart ? "Hide Graph" : "Show Graph", systemImage: "chart.pie")
            }
        }
    
        ToolbarItem(id: "add", placement: .primaryAction) {
            Menu {
                Button("Bill") {
                    addBill(.bill)
                }
                
                Button("Subscription") {
                    addBill(.subscription)
                }
                
                Button("Utility") {
                    addBill(.utility)
                }
            } label: {
                Label("Add", systemImage: "plus")
            }
        }
        
        GeneralIEToolbarButton(on: bills, selection: $selection, inspect: inspect, warning: warning, role: .edit, placement: .primaryAction)
        GeneralIEToolbarButton(on: bills, selection: $selection, inspect: inspect, warning: warning, role: .inspect, placement: .primaryAction)
        GeneralDeleteToolbarButton(on: bills, selection: $selection, delete: deleting, warning: warning, placement: .primaryAction)
    }
    
    @ViewBuilder
    private var chartView: some View {
        VStack {
            Chart(bills) { bill in
                SectorMark(
                    angle: .value(
                        Text(verbatim: bill.name),
                        bill.pricePer(showcasePeriod)
                    )
                ).foregroundStyle(by: .value(
                    Text(verbatim: bill.name),
                    bill.name
                )
                )
            }.frame(minHeight: 350)
            
            HStack {
                Spacer()
                Button("Ok", action: { showingChart = false } ).buttonStyle(.borderedProminent)
            }
        }.padding()
    }
    
    var body: some View {
        VStack {
            wide
            
            HStack {
                Spacer()
                Text("Total:")
                Text(self.totalPPP, format: .currency(code: currencyCode))

                Picker("/", selection: $showcasePeriod) {
                    ForEach(TimePeriods.allCases, id: \.id) { period in
                        Text(period.perName).tag(period)
                    }
                }
                .frame(width: 150)
            }
        }.sheet(item: $inspect.value) { bill in
            ElementIE(bill, mode: inspect.mode)
        }.toolbar(id: "billsToolbar") {
            toolbar
        }.alert("Warning", isPresented: $warning.isPresented, actions: {
            Button("Ok") {
                warning.isPresented = false
            }
        }, message: {
            Text((warning.warning ?? .noneSelected).message )
        })
        .confirmationDialog("deleteItemsConfirm", isPresented: $deleting.isDeleting, titleVisibility: .visible) {
            DeletingActionConfirm(deleting)
        }.sheet(isPresented: $showingChart) {
            chartView
        }.padding()
            .toolbarRole(.automatic)
            .navigationTitle("Bills")
    }
}

#Preview(traits: .sampleData) {
    NavigationStack {
        AllBillsViewEdit()
    }
}
