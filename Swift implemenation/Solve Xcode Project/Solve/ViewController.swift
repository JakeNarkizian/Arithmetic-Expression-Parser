//
//  ViewController.swift
//  Solve
//

import Cocoa

class ViewController: NSViewController {

    // --- OUTLETS ---
    @IBOutlet weak var expression: NSTextField!
    @IBOutlet weak var equation_body: NSTextField!
    @IBOutlet weak var equation_eq: NSTextField!
    @IBOutlet weak var expr_eq: NSTextField!
    @IBOutlet weak var x_eql: NSTextField!
    @IBOutlet weak var expression_error: NSTextField!
    @IBOutlet weak var equation_error: NSTextField!
    
    
    // --- ACTIONS ---
    @IBAction func solve_equation(sender: AnyObject) {
        
        // --- CHECK VALID ---
        if equation_body.stringValue == ""{
            equation_body.placeholderString = "Please Enter a Valid Equation!"
            return
        }
        if equation_eq.stringValue == ""{
            return
        }
        
        // --- SOLVE ---
        var equation = OpList(eql_num: (equation_eq.stringValue as NSString).doubleValue)
        
        let error:String? = equation.argToOpList(equation_body.stringValue)
        if error != nil{
            equation_error.stringValue = "ERROR: " + error!
            x_eql.stringValue = ":-("
            return
        }
        
        // --- CHANGE VIEW ---
        var answer = String(format:"%.2f",equation.solve())
        if answer == "-0.00"{
            answer = "0.00"
        }
        x_eql.stringValue = answer
        equation_error.stringValue = ""

    }
    
    @IBAction func evaluate_expression(sender: AnyObject) {
        
        // --- CHECK VALID ---
        if expression.stringValue == ""{
            expression.placeholderString = "Please Enter a Valid Expression!"
            return
        }
        
        // --- SOLVE ---
        var express = OpList()
        
        let error:String? = express.argToOpList(expression.stringValue)
        if error != nil{
            expression_error.stringValue = "ERROR: " + error!
            expr_eq.stringValue = ":-("
            return
        }

        // --- CHANGE VIEW ---
        var answer = String(format:"%.2f",express.eval(0.0))
        if answer == "-0.00"{
            answer = "0.00"
        }
        expr_eq.stringValue = answer
        expression_error.stringValue = ""
    }
    
    
    
    
    
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }


}

