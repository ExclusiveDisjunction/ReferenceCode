//
//  TopicGroup.swift
//  Austin
//
//  Created by Hollan Sellars on 6/30/25.
//

import Foundation

public struct DocumentElementID : RawRepresentable, Hashable, Sendable {
    public typealias RawValue = String
    
    public init() {
        self.parts = []
    }
    public init(rawValue: String) {
        self.parts = rawValue.split(separator: " ").map { $0.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() }
    }
    public init(parts: [String]) {
        self.parts = parts.map { $0.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() }
    }
    
    public var parts: [String];
    public var rawValue: String {
        parts.joined(separator: "/")
    }
    public var name: String {
        parts.last ?? ""
    }
}

public enum HeaderSize : Hashable, Equatable, Codable, Sendable {
    case header1, header2, header3, header4
}

@Observable
public final class HeaderContent : Identifiable, Codable {
    public convenience init(size: HeaderSize) {
        self.init(text: "", size: size)
    }
    public init(text: String, size: HeaderSize, id: UUID = UUID()) {
        self.id = id
        self.text = text
        self.size = size
    }
    
    public var id: UUID;
    public var text: String;
    public var size: HeaderSize;
}

@Observable
public final class TextContent : Identifiable, Codable {
    public init(text: String = "", id: UUID = UUID()) {
        self.id = id
        self.text = text
    }
    
    public var id: UUID;
    public var text: String;
}

@Observable
public final class CodingContent: Identifiable, Codable {
    public init(language: String, code: String = "", id: UUID = UUID()) {
        self.language = language
        self.code = code
        self.id = id
    }
    
    public var id: UUID;
    public var code: String;
    public var language: String;
}

@Observable
public final class MathContent : Identifiable, Codable {
    public init(latex: String = "", id: UUID = UUID()) {
        self.id = id
        self.latex = latex
    }
    public var id: UUID;
    public var latex: String;
}

@Observable
public final class ImageContent: Identifiable, Codable {
    public init(imageID: UUID, size: CGSize? = nil, caption: String = "", id: UUID = UUID()) {
        self.id = id
        self.imageID = imageID
        self.size = size
        self.caption = caption
    }
    
    public var id: UUID;
    public var imageID: UUID;
    public var size: CGSize?;
    public var caption: String;
}

public enum TopicContent: Identifiable, Codable {
    case header(HeaderContent)
    case body(TextContent)
    case code(CodingContent)
    case math(MathContent)
    case image(ImageContent)
    
    public var id: UUID {
        switch self {
            case .header(let h): h.id
            case .body(let b): b.id
            case .code(let c): c.id
            case .math(let m): m.id
            case .image(let i): i.id
        }
    }
}

