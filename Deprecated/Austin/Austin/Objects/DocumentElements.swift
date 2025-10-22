//
//  DocumentElements.swift
//  Austin
//
//  Created by Hollan Sellars on 7/1/25.
//

import Foundation

public enum DocumentElement : Identifiable {
    case topic(Topic)
    case group(TopicGroup)
    
    public var isTopic: Bool {
        switch self {
            case .group(_): false
            case .topic(_): true
        }
    }
    public var isGroup: Bool {
        switch self {
            case .group(_): true
            case .topic(_): false
        }
    }
    
    public func walkFind(id: UUID) -> DocumentElement? {
        if id == self.id {
            return self
        }
        
        if case .group(let topicGroup) = self {
            return topicGroup.children.walkFind(id: id)
        }
        
        return nil
    }
    
    public var id: UUID {
        get {
            switch self {
                case .group(let g): g.id
                case .topic(let t): t.id
            }
        }
        set {
            switch self {
                case .group(let g): g.id = newValue
                case .topic(let t): t.id = newValue
            }
        }
    }
    public var name: String {
        get {
            switch self {
                case .group(let g): g.name
                case .topic(let t): t.name
            }
        }
        set {
            switch self {
                case .group(let g): g.name = newValue
                case .topic(let t): t.name = newValue
            }
        }
    }
    
    public var children: [DocumentElement]? {
        switch self {
            case .group(let g): g.children
            case .topic(_): nil
        }
    }
}
extension Array where Element == DocumentElement {
    public func walkFind(id: UUID) -> DocumentElement? {
        for item in self {
            if let result = item.walkFind(id: id) {
                return result
            }
        }
        
        return nil
    }
}

@Observable
public class DocumentElementBase : Identifiable, Codable {
    public init(name: String, parent: DocumentElementBase? = nil, id: UUID = UUID()) {
        self.name = name
        self.parent = parent
        self.id = id
    }
    
    public var id: UUID;
    public var name: String;
    public weak var parent: DocumentElementBase?;
}

@Observable
public final class Topic : Identifiable, Codable {
    public init(name: String, content: [TopicContent] = [], id: UUID = UUID()) {
        self.content = content
        self.name = name
        self.id = id
    }
    
    public var id: UUID;
    public var name: String;
    public var content: [TopicContent];
    
}

@Observable
public final class TopicGroup : Identifiable {
    public init(name: String, subGroups: [TopicGroup] = [], topics: [Topic] = [], id: UUID = UUID()) {
        self.id = id
        self.name = name
        self.subGroups = subGroups
        self.topics = topics
    }
    
    public var id: UUID;
    public var name: String;
    public var subGroups: [TopicGroup];
    public var topics: [Topic];
    
    public var children: [DocumentElement] {
        subGroups.map { .group($0) } + topics.map { .topic($0) }
    }
}

