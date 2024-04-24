import UIKit
import CoreGraphics

typealias EnumChangedHandler = (_ newValue: String) -> Void

class LEEnumPickerVC: UIViewController, UIPickerViewDelegate, UIPickerViewDataSource {
    
    private let BackgroundAlpha: CGFloat = 0.2
    private let HeaderTitleFontSize: CGFloat = 24
    private let ContainerViewCornerRadius: CGFloat = 36
    private let ContainerViewWidth: CGFloat = 327
    private let ContainerViewHeightInset: CGFloat = 100
    private let DoneButtonWidth: CGFloat = 122
    private let DoneButtonHeight: CGFloat = 56
    private let DoneButtonOffset: CGFloat = 12
    private let DoneButtonCornerRadius: CGFloat = 40
    private let SpaceBetween: CGFloat = 20
    
    private let periodPickerView = UIPickerView(frame: .zero)
    
    var enumChangedHandler: EnumChangedHandler?
    let enumList: [String]
    let initialValueIndex: Int
    
    private let titleLabel = UILabel(frame: .zero)
    
    init(title: String, enumList: [String], initialValueIndex: Int) {
        self.initialValueIndex = initialValueIndex
        self.enumList = enumList
        
        super.init(nibName: nil, bundle: nil)
        
        self.titleLabel.text = title
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    // MARK: - Routine -
    
    private func createLayout() {
        view.backgroundColor = UIColor.black.withAlphaComponent(0.7)
        
        periodPickerView.dataSource = self
        periodPickerView.delegate = self
        periodPickerView.translatesAutoresizingMaskIntoConstraints = false
        periodPickerView.selectRow(initialValueIndex, inComponent: 0, animated: false)
        
        let viewContainer = UIView(frame: .zero)
        viewContainer.translatesAutoresizingMaskIntoConstraints = false
        viewContainer.backgroundColor = .white
        viewContainer.layer.cornerRadius = ContainerViewCornerRadius
        
        let doneButton = UIButton(frame: .zero)
        doneButton.translatesAutoresizingMaskIntoConstraints = false
        doneButton.setTitle("settings.ok".localized(), for: .normal)
        doneButton.addTarget(self, action: #selector(closePeriodPicker), for: .touchUpInside)
        doneButton.backgroundColor = UIColor(named: "LunaBlue")
        doneButton.layer.cornerRadius = ceil(DoneButtonHeight / 2.0)
        
        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        titleLabel.numberOfLines = 0
        titleLabel.font = Fonts.etelka_17
        titleLabel.textAlignment = .center
        
        let pickerView = UIPickerView(frame: .zero)
        pickerView.delegate = self
        pickerView.dataSource = self
        pickerView.translatesAutoresizingMaskIntoConstraints = false
        
        viewContainer.addSubview(pickerView)
        viewContainer.addSubview(doneButton)
        viewContainer.addSubview(titleLabel)
        view.addSubview(viewContainer)
        
        var labelFitSize = view.bounds.size
        labelFitSize.width = ContainerViewWidth - 4 * DoneButtonOffset
        
        var containerHeight = pickerView.intrinsicContentSize.height
        containerHeight += DoneButtonHeight
        containerHeight += titleLabel.sizeThatFits(labelFitSize).height
        containerHeight += 3 * ContainerViewCornerRadius
        containerHeight += ceil(ContainerViewCornerRadius / 2)
        NSLayoutConstraint.activate([
            titleLabel.topAnchor.constraint(equalTo: viewContainer.topAnchor, constant: ContainerViewCornerRadius),
            titleLabel.leadingAnchor.constraint(equalTo: viewContainer.leadingAnchor, constant: 2 * DoneButtonOffset),
            titleLabel.trailingAnchor.constraint(equalTo: viewContainer.trailingAnchor, constant: -2 * DoneButtonOffset),
            
            pickerView.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: ContainerViewCornerRadius),
            pickerView.heightAnchor.constraint(equalToConstant: pickerView.intrinsicContentSize.height),
            pickerView.leadingAnchor.constraint(equalTo: viewContainer.leadingAnchor),
            pickerView.trailingAnchor.constraint(equalTo: viewContainer.trailingAnchor),
            
            doneButton.topAnchor.constraint(equalTo: pickerView.bottomAnchor, constant: ContainerViewCornerRadius),
            doneButton.trailingAnchor.constraint(equalTo: viewContainer.trailingAnchor, constant: -DoneButtonOffset),
            doneButton.heightAnchor.constraint(equalToConstant: DoneButtonHeight),
            doneButton.widthAnchor.constraint(equalToConstant: DoneButtonWidth),
            
            viewContainer.centerYAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerYAnchor),
            viewContainer.centerXAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerXAnchor),
            viewContainer.widthAnchor.constraint(equalToConstant: ContainerViewWidth),
            viewContainer.heightAnchor.constraint(equalToConstant: ceil(containerHeight)),
        ])
    }
    
    // MARK: - Handler -
    
    @objc
    private func closePeriodPicker() {
        view.removeFromSuperview()
        removeFromParent()
    }
    
    // MARK: - UIPickerViewDataSource -
    
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return enumList.count
    }
    
    // MARK: - UIPickerViewDelegate -
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return enumList[row].localized()
    }
    
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        enumChangedHandler?(enumList[row])
    }
    
}
