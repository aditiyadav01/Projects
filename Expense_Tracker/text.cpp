#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <string>

struct Expense
{
	std::string category;
	std::string date; 
	double amount;
	std::string description;
};

class ExpenseTracker
{
private:
	std::vector<Expense> expenses;
	std::unordered_map<std::string, double> categoryTotals;
	double monthlyBudget;

	void updateCategoryTotals()
	{
		categoryTotals.clear();
		for (const auto &expense : expenses)
		{
			categoryTotals[expense.category] += expense.amount;
		}
	}

public:
	ExpenseTracker() : monthlyBudget(0.0) {}

	void setMonthlyBudget(double budget)
	{
		monthlyBudget = budget;
		std::cout << "Monthly budget set to " << monthlyBudget << "\n";
	}

	void addExpense(const Expense &expense)
	{
		expenses.push_back(expense);
		categoryTotals[expense.category] += expense.amount;
		std::cout << "Expense added successfully!\n";
	}

	void viewExpenses() const
	{
		if (expenses.empty())
		{
			std::cout << "No expenses to display.\n";
			return;
		}

		std::cout << "\nExpenses:\n";
		std::cout << std::setw(15) << "Category" << std::setw(15) << "Date"
				  << std::setw(10) << "Amount" << "   Description\n";
		std::cout << "-----------------------------------------------------------\n";
		for (const auto &expense : expenses)
		{
			std::cout << std::setw(15) << expense.category << std::setw(15)
					  << expense.date << std::setw(10) << expense.amount
					  << "   " << expense.description << "\n";
		}
	}

	void analyzeExpenses() const
	{
		if (expenses.empty())
		{
			std::cout << "No expenses to analyze.\n";
			return;
		}

		std::cout << "\nExpense Analysis:\n";
		std::cout << "Category-wise Spending:\n";
		for (const auto &pair : categoryTotals)
		{
			std::cout << "  " << pair.first << ": " << pair.second << "\n";
		}

		double totalSpent = 0.0;
		for (const auto &expense : expenses)
		{
			totalSpent += expense.amount;
		}
		std::cout << "Total Spent: " << totalSpent << "\n";
		std::cout << "Remaining Budget: " << (monthlyBudget - totalSpent) << "\n";
	}

	void saveToFile(const std::string &filename) const
	{
		std::ofstream outFile(filename);
		if (!outFile)
		{
			std::cerr << "Error saving to file.\n";
			return;
		}

		outFile << monthlyBudget << "\n";
		for (const auto &expense : expenses)
		{
			outFile << expense.category << "\n"
					<< expense.date << "\n"
					<< expense.amount << "\n"
					<< expense.description << "\n";
		}

		outFile.close();
		std::cout << "Expenses saved to " << filename << "\n";
	}

	void loadFromFile(const std::string &filename)
	{
		std::ifstream inFile(filename);
		if (!inFile)
		{
			std::cerr << "No saved file found. Starting fresh.\n";
			return;
		}

		expenses.clear();
		categoryTotals.clear();

		inFile >> monthlyBudget;
		inFile.ignore();

		Expense expense;
		while (std::getline(inFile, expense.category))
		{
			std::getline(inFile, expense.date);
			inFile >> expense.amount;
			inFile.ignore();
			std::getline(inFile, expense.description);
			expenses.push_back(expense);
			categoryTotals[expense.category] += expense.amount;
		}

		inFile.close();
		std::cout << "Expenses loaded from " << filename << "\n";
	}
};

int main()
{
	ExpenseTracker tracker;
	tracker.loadFromFile("expenses.txt");

	int choice;
	do
	{
		std::cout << "\nPersonal Expense Tracker\n";
		std::cout << "1. Set Monthly Budget\n";
		std::cout << "2. Add Expense\n";
		std::cout << "3. View Expenses\n";
		std::cout << "4. Analyze Expenses\n";
		std::cout << "5. Save and Exit\n";
		std::cout << "Enter your choice: ";
		std::cin >> choice;

		switch (choice)
		{
		case 1:
		{
			double budget;
			std::cout << "Enter monthly budget: ";
			std::cin >> budget;
			tracker.setMonthlyBudget(budget);
			break;
		}
		case 2:
		{
			Expense expense;
			std::cin.ignore();
			std::cout << "Enter category: ";
			std::getline(std::cin, expense.category);
			std::cout << "Enter date (YYYY-MM-DD): ";
			std::getline(std::cin, expense.date);
			std::cout << "Enter amount: ";
			std::cin >> expense.amount;
			std::cin.ignore();
			std::cout << "Enter description: ";
			std::getline(std::cin, expense.description);
			tracker.addExpense(expense);
			break;
		}
		case 3:
			tracker.viewExpenses();
			break;
		case 4:
			tracker.analyzeExpenses();
			break;
		case 5:
			tracker.saveToFile("expenses.txt");
			std::cout << "Goodbye!\n";
			break;
		default:
			std::cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 5);

	return 0;
}