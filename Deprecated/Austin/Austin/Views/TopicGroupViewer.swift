//
//  TopicGroupViewer.swift
//  Austin
//
//  Created by Hollan Sellars on 7/1/25.
//

import SwiftUI

struct TopicGroupViewer : View {
    @Bindable var data: TopicGroup
    
    var body: some View {
        VStack {
            
        }.navigationTitle(data.name)
    }
}
