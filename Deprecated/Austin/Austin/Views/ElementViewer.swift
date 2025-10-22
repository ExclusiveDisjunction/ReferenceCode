//
//  ElementViewer.swift
//  Austin
//
//  Created by Hollan Sellars on 7/1/25.
//

import SwiftUI

struct ElementViewer : View {
    var data: DocumentElement
    
    var body: some View {
        switch data {
            case .group(let g): TopicGroupViewer(data: g)
            case .topic(let t): TopicViewer(data: t)
        }
    }
}
