//
//  AustinDocument.swift
//  Austin
//
//  Created by Hollan Sellars on 6/30/25.
//

import SwiftUI
import UniformTypeIdentifiers

extension UTType {
    static var austinProject: UTType {
        UTType(exportedAs: "com.exdisj.austinproj")
    }
}

@Observable
public final class Workspace : Codable {
    public init() {
        self.currentlyOpen = .init()
    }
    
    public var currentlyOpen: Set<DocumentElement.ID>
}

@Observable
public final class DocumentContent : @unchecked Sendable {
    public init(topics: [Topic] = [], groups: [TopicGroup] = [], workspace: Workspace = .init()) {
        self.rootGroups = groups
        self.rootTopics = topics
        self.workspace = workspace
    }
    public init(wrapper: FileWrapper) throws(LoadError) {
        if let workspaceData = wrapper.fileWrappers?["workspace.json"]?.regularFileContents {
            do {
                workspace = try JSONDecoder().decode(Workspace.self, from: workspaceData)
            }
            catch let e {
                throw LoadError(target: "Workspace", reason: e.localizedDescription)
            }
        }
        else {
            workspace = .init() //We can just make a new one, not that deep
        }
        
        if let documentsWrapper = wrapper.fileWrappers?[Self.rootName] {
            guard documentsWrapper.isDirectory else {
                throw LoadError(target: "Main Group", reason: "The file wrapper exists, but it is not of directory kind.")
            }
            
            let rootGroup = try Self.readGroup(wrapper: documentsWrapper)
            self.rootGroups = rootGroup.subGroups
            self.rootTopics = rootGroup.topics
        }
        else {
            self.rootGroups = []
            self.rootTopics = []
        }
    }
    
    private static func readGroup(wrapper: FileWrapper) throws(LoadError) -> TopicGroup {
        var groupResult: [TopicGroup] = []
        var topicResult: [Topic] = []
        
        guard let contents = wrapper.fileWrappers, wrapper.isDirectory else { throw .init(target: "Group", reason: "Expected the group to be a directory, but it was not") }
        
        for (name, file) in contents {
            if file.isDirectory {
                let child = try readGroup(wrapper: file)
                groupResult.append(child)
            }
            else if let data = file.regularFileContents {
                let content: Topic;
                do {
                    content = try JSONDecoder().decode(Topic.self, from: data)
                }
                catch let e {
                    throw .init(target: name, reason: e.localizedDescription)
                }
                
                topicResult.append(content)
            }
            else {
                throw .init(target: name, reason: "Expected either a group or topic, but got something that is neither")
            }
        }
        
        guard let name = wrapper.filename else {
            throw .init(target: "Group", reason: "The group's name could not be determined")
        }
        
        return .init(name: name, subGroups: groupResult, topics: topicResult)
    }
    public static func writeGroup(group: TopicGroup) throws -> FileWrapper {
        var wrappers: [String: FileWrapper] = .init()
        
        for element in group.children {
            switch element {
                case .group(let g):
                    wrappers[g.name] = try Self.writeGroup(group: g)
                case .topic(let t):
                    let data = try JSONEncoder().encode(t)
                    wrappers[t.name] = .init(regularFileWithContents: data)
            }
        }
        
        return FileWrapper(directoryWithFileWrappers: wrappers)
    }
    
    public var workspace: Workspace;
    public var rootGroups: [TopicGroup]
    public var rootTopics: [Topic]
    public var rootElements: [DocumentElement] {
        rootGroups.map { .group($0) } + rootTopics.map { .topic($0) }
    }
    public static let rootName: String = "root"
    public var rootElement: TopicGroup {
        .init(name: Self.rootName, subGroups: rootGroups, topics: rootTopics)
    }
    
    public func write(wrapper: FileDocumentWriteConfiguration) throws -> FileWrapper {
        var files: [String: FileWrapper] = .init()
        
        let workspaceData = try JSONEncoder().encode(workspace)
        files["workspace.json"] = .init(regularFileWithContents: workspaceData)
        
        let group = rootElement;
        let mainGroup = try Self.writeGroup(group: group)
        files[group.name] = mainGroup
        
        return .init(directoryWithFileWrappers: files)
    }
}

public struct LoadError : Error, Sendable {
    public let target: String;
    public let reason: String;
    
    public var localizedDescription: String {
        "When loading '\(target)', an error occured. Reason: '\(reason)'."
    }
}

struct AustinDocument: FileDocument {
    /// Content to place in a gitignore to block out the workspace from being looked at all the time.
    private static let gitignoreContent = "workspace.json";
    
    public static let exampleDocument: AustinDocument = .init(
        content: .init(
            topics: [
                .init(
                    name: "Welcome",
                    content: [
                        .header(.init(text: "Hello!", size: .header1)),
                        .body(.init(text: "Welcome to Austin! This is an example document showing some of the features it has to offer.")),
                        .body(.init(text: "Please look around!"))
                    ]
                )
            ],
            groups: [
                .init(
                    name: "Example Group",
                    subGroups: [
                        .init(name: "Child Group")
                    ],
                    topics: [
                        .init(
                            name: "Example Other Topic",
                            content: [
                                .body(.init(text: "You found me..."))
                            ]
                        )
                    ]
                )
            ]
        )
    )
    
    var content: DocumentContent

    init(content: DocumentContent) {
        self.content = content
    }
    init(configuration: ReadConfiguration) throws(LoadError) {
        self.content = try .init(wrapper: configuration.file)
    }

    static var readableContentTypes: [UTType] { [.austinProject] }
    static var writableContentTypes: [UTType] { [.austinProject] }
    
    func fileWrapper(configuration: WriteConfiguration) throws -> FileWrapper {
        try self.content.write(wrapper: configuration)
    }
}
