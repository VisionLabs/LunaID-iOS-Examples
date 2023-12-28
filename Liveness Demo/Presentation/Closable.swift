//
//  Closable.swift
//  Liveness Demo
//
//  Created by Иван Казанцев on 24.09.2023.
//

protocol Closable {

    func configure(withCloseBlock closeBlock: @escaping () -> Void)
}
