//
//  TopicViewer.swift
//  Austin
//
//  Created by Hollan Sellars on 7/1/25.
//

import SwiftUI

struct TopicViewer : View {
    @Bindable var data: Topic
    @State private var isEdit: Bool = false;
    
    var body: some View {
        VStack {
            
        }.navigationTitle(data.name)
        /*
            .toolbar {
                ToolbarItem(placement: .primaryAction) {
                    Menu {
                        
                    } label: {
                        Label("Add", systemImage: "plus")
                    }
                }
                
                ToolbarItem(placement: .primaryAction) {
                    Button {
                        isEdit.toggle()
                    } label: {
                        Label(isEdit ? "Preview" : "Edit", systemImage: isEdit ? "text.page.badge.magnifyingglass" : "pencil")
                    }
                }
            }
         */
    }
}
