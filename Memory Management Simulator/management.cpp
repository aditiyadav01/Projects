#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent a memory block
struct MemoryBlock {
    int size;
    int id;
    bool allocated;

    MemoryBlock(int id, int size) : id(id), size(size), allocated(false) {}
};

// Structure to represent a process
struct Process {
    int id;
    int size;
    int allocatedBlock;

    Process(int id, int size) : id(id), size(size), allocatedBlock(-1) {}
};

// Function to allocate memory using First Fit
void firstFit(vector<MemoryBlock>& blocks, Process& process) {
    for (auto& block : blocks) {
        if (!block.allocated && block.size >= process.size) {
            block.allocated = true;
            process.allocatedBlock = block.id;
            cout << "Process " << process.id << " allocated to block of size " << block.size << endl;
            return;
        }
    }
    cout << "Process " << process.id << " cannot be allocated (First Fit)\n";
}

// Function to allocate memory using Best Fit
void bestFit(vector<MemoryBlock>& blocks, Process& process) {
    int bestIdx = -1;
    for (int i = 0; i < blocks.size(); i++) {
        if (!blocks[i].allocated && blocks[i].size >= process.size) {
            if (bestIdx == -1 || blocks[i].size < blocks[bestIdx].size) {
                bestIdx = i;
            }
        }
    }
    if (bestIdx != -1) {
        blocks[bestIdx].allocated = true;
        process.allocatedBlock = blocks[bestIdx].id;
        cout << "Process " << process.id << " allocated to best fit block of size " << blocks[bestIdx].size << endl;
    } else {
        cout << "Process " << process.id << " cannot be allocated (Best Fit)\n";
    }
}

// Function to allocate memory using Worst Fit
void worstFit(vector<MemoryBlock>& blocks, Process& process) {
    int worstIdx = -1;
    for (int i = 0; i < blocks.size(); i++) {
        if (!blocks[i].allocated && blocks[i].size >= process.size) {
            if (worstIdx == -1 || blocks[i].size > blocks[worstIdx].size) {
                worstIdx = i;
            }
        }
    }
    if (worstIdx != -1) {
        blocks[worstIdx].allocated = true;
        process.allocatedBlock = blocks[worstIdx].id;
        cout << "Process " << process.id << " allocated to worst fit block of size " << blocks[worstIdx].size << endl;
    } else {
        cout << "Process " << process.id << " cannot be allocated (Worst Fit)\n";
    }
}

// Function to display memory block status
void displayMemoryStatus(vector<MemoryBlock>& blocks) {
    cout << "\nMemory Blocks Status:\n";
    for (auto& block : blocks) {
        cout << "Block " << block.id << ": Size = " << block.size
             << (block.allocated ? ", Allocated" : ", Not Allocated") << endl;
    }
    cout << endl;
}

int main() {
    // Define memory blocks
    vector<MemoryBlock> blocks = {
        {1, 100}, {2, 200}, {3, 300}, {4, 400}, {5, 500}
    };

    // Define processes
    vector<Process> processes = {
        {1, 200}, {2, 100}, {3, 500}, {4, 400}, {5, 300}
    };

    // Allocate memory using First Fit
    cout << "\nAllocating using First Fit:\n";
    for (auto& process : processes) {
        firstFit(blocks, process);
    }
    displayMemoryStatus(blocks);

    // Reset memory blocks
    for (auto& block : blocks) block.allocated = false;

    // Allocate memory using Best Fit
    cout << "\nAllocating using Best Fit:\n";
    for (auto& process : processes) {
        bestFit(blocks, process);
    }
    displayMemoryStatus(blocks);

    // Reset memory blocks
    for (auto& block : blocks) block.allocated = false;

    // Allocate memory using Worst Fit
    cout << "\nAllocating using Worst Fit:\n";
    for (auto& process : processes) {
        worstFit(blocks, process);
    }
    displayMemoryStatus(blocks);

    return 0;
}
