//
//  ContentView.swift
//  Austin
//
//  Created by Hollan Sellars on 6/30/25.
//

import SwiftUI

public struct TargetElementKey : FocusedValueKey {
    public typealias Value = DocumentElement?;
}
public struct CurrentTopicKey : FocusedValueKey {
    public typealias Value = Topic?;
}
public struct RootDocumentKey : FocusedValueKey {
    public typealias Value = DocumentContent?;
}
public struct BindingKey<T> : FocusedValueKey {
    public typealias Value = Binding<T?>;
}
public extension FocusedValues {
    var targetElement: DocumentElement? {
        get {
            if let value = self[TargetElementKey.self] {
                return value
            }
            else {
                return nil
            }
        }
        set {
            self[TargetElementKey.self] = newValue
        }
    }
    var targetElementId: Binding<DocumentElement.ID?>? {
        get { self[BindingKey<DocumentElement.ID>.self] }
        set { self[BindingKey<DocumentElement.ID>.self] = newValue }
    }
    var root : DocumentContent? {
        get {
            if let value = self[RootDocumentKey.self] {
                return value
            }
            else {
                return nil
            }
        }
        set {
            self[RootDocumentKey.self] = newValue
        }
    }
    var currentTopic: Topic? {
        get { self[CurrentTopicKey.self] ?? nil }
        set { self[CurrentTopicKey.self] = newValue }
    }
}

public extension DocumentElement? {
    var isSelected: Bool {
        self != nil
    }
    var isTopic: Bool {
        if let item = self {
            item.isTopic
        }
        else {
            false
        }
    }
    var isGroup: Bool {
        if let item = self {
            item.isGroup
        }
        else {
            false
        }
    }
}

struct MainCommands : Commands {
    @FocusedValue(\.root) private var root: DocumentContent?;
    @FocusedValue(\.targetElementId) private var targetId: Binding<DocumentElement.ID?>?;
    @FocusedValue(\.targetElement) private var target: DocumentElement?;
    
    var body: some Commands {
        CommandMenu("Document") {
            Button {
                let new = Topic(name: "New Topic")
                
                if let target = target {
                    if case .group(let g) = target {
                        g.topics.append(new)
                    }
                }
                else if let root = root {
                    root.rootTopics.append(new)
                }
                
                if let binding = targetId {
                    binding.wrappedValue = new.id
                }
            } label: {
                Label("New Topic", systemImage: "document.badge.plus")
            }.disabled(target.isTopic)
                .keyboardShortcut("t", modifiers: [.command, .option])
            
            Button {
                let new = TopicGroup(name: "New Group")
                
                if let target = target {
                    if case .group(let g) = target {
                        g.subGroups.append(new)
                    }
                }
                else if let root = root {
                    root.rootGroups.append(new)
                }
                
                if let binding = targetId {
                    binding.wrappedValue = new.id
                }
            } label: {
                Label("New Group", systemImage: "folder.badge.plus")
            }.disabled(target.isTopic)
                .keyboardShortcut("g", modifiers: [.command, .option])
        }
    }
}

struct OpenTopic : Identifiable{
    init(name: String, id: UUID) {
        self.name = name
        self.id = id
    }
    init(from: Topic) {
        self.name = from.name
        self.id = from.id
    }
    
    var name: String;
    var id: UUID;
}

struct DocumentContentViewer : View {
    @Binding var document: AustinDocument;
    @Binding var selection: Topic.ID?;
    @Binding var openTabs: [OpenTopic];
    @State private var currentTopic: Topic?;
    
    var body: some View {
        VStack {
            ScrollView {
                HStack {
                    ForEach(openTabs) { topic in
                        HStack {
                            Text(topic.name)
                            
                            Button {
                                withAnimation {
                                    openTabs.removeAll(where: { $0.id == topic.id })
                                }
                            } label: {
                                Image(systemName: "x")
                            }.padding(.leading, 2)
                        }.onTapGesture {
                            selection = topic.id
                        }.padding([.leading, .trailing], 2)
                    }
                }
            }
            
            if let topic = currentTopic {
                TopicViewer(data: topic)
            }
            else {
                Text("No editor open")
                    .italic()
            }
        }.focusedValue(\.currentTopic, currentTopic)
    }
}

struct MainView: View {
    @Binding var document: AustinDocument
    
    @State private var listSelection: DocumentElement.ID?;
    @State private var topicSelection: DocumentElement.ID?;
    @State private var openTabs: [OpenTopic] = [];
    @State private var target: DocumentElement?;
    @State private var showInspector = false;
    
    public var somethingSelected: Bool {
        target != nil
    }
    public var currentIsGroup : Bool {
        guard let target = target else { return false }
        
        return target.isGroup
    }
    public var currentIsTopic: Bool {
        guard let target = target else { return false }
        
        return target.isTopic
    }

    var body: some View {
        NavigationSplitView {
            List(document.content.rootElements, children: \.children, selection: $selection) { element in
                switch element {
                    case .group(let g): HStack {
                        Image(systemName: g.children.isEmpty ? "folder" : "folder.fill")
                        
                        Text(g.name)
                    }
                    case .topic(let t): HStack {
                        Image(systemName: "document.text")
                        
                        Text(element.name)
                            .onTapGesture(count: 2) {
                                
                            }
                    }
                }
            }.contextMenu(forSelectionType: DocumentElement.ID.self) { selection in
                Button {
                    
                } label: {
                    Label("Add Topic", systemImage: "document.badge.plus")
                }
                
                Button {
                    
                } label: {
                    Label("Add Group", systemImage: "folder.badge.plus")
                }
            }
        } detail: {
            
        }.onChange(of: selection) { _, newValue in
            if let id = newValue {
                target = document.content.rootElements.walkFind(id: id)
            }
            else {
                target = nil
            }
        }
        .focusedValue(\.targetElement, target)
        .focusedValue(\.root, document.content)
        .focusedValue(\.targetElementId, $selection)
        .inspector(isPresented: $showInspector) {
            
        }
        .toolbar {
            ToolbarItem(placement: .primaryAction) {
                Button {
                    showInspector.toggle()
                } label: {
                    Label(showInspector ? "Hide Inspector" : "Show Inspector", systemImage: "sidebar.right")
                }
            }
        }.inspectorColumnWidth(min: 70, ideal: 100, max: 120)
    }
}

#Preview {
    var document = AustinDocument.exampleDocument
    let binding = Binding(get: { document }, set: { document = $0 } )
    MainView(document: binding)
        
}
