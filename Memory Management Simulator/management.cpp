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
            cout << "Process " << process.id << " allocated to block " << block.id << " (Size: " << block.size << ")\n";
            return;
        }
    }
    cout << "Process " << process.id << " cannot be allocated (First Fit)\n";
}

// Function to allocate memory using Best Fit with splitting
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
        if (blocks[bestIdx].size > process.size) {
            blocks.push_back(MemoryBlock(blocks.size() + 1, blocks[bestIdx].size - process.size));
        }
        blocks[bestIdx].size = process.size;
        blocks[bestIdx].allocated = true;
        process.allocatedBlock = blocks[bestIdx].id;
        cout << "Process " << process.id << " allocated to best fit block " << blocks[bestIdx].id << " (Size: " << process.size << ")\n";
    } else {
        cout << "Process " << process.id << " cannot be allocated (Best Fit)\n";
    }
}

// Function to allocate memory using Worst Fit with retry mechanism
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
        cout << "Process " << process.id << " allocated to worst fit block " << blocks[worstIdx].id << " (Size: " << blocks[worstIdx].size << ")\n";
    } else {
        cout << "Process " << process.id << " cannot be allocated (Worst Fit), retrying...\n";
        for (auto& block : blocks) {
            if (!block.allocated && block.size >= process.size) {
                block.allocated = true;
                process.allocatedBlock = block.id;
                cout << "Process " << process.id << " reallocated to block " << block.id << " (Size: " << block.size << ")\n";
                return;
            }
        }
        cout << "Process " << process.id << " still cannot be allocated.\n";
    }
}

// Function to deallocate memory
void deallocateMemory(vector<MemoryBlock>& blocks, Process& process) {
    for (auto& block : blocks) {
        if (block.id == process.allocatedBlock) {
            block.allocated = false;
            cout << "Process " << process.id << " deallocated from block " << block.id << "\n";
            process.allocatedBlock = -1;
            return;
        }
    }
}

// Function to display memory block status
void displayMemoryStatus(vector<MemoryBlock>& blocks) {
    cout << "\nMemory Blocks Status:" << endl;
    for (auto& block : blocks) {
        cout << "Block " << block.id << " | Size: " << block.size << " | "
             << (block.allocated ? "Allocated" : "Free") << endl;
    }
    cout << endl;
}

int main() {
    vector<MemoryBlock> blocks = {{1, 100}, {2, 200}, {3, 300}, {4, 400}, {5, 500}};
    vector<Process> processes = {{1, 200}, {2, 100}, {3, 500}, {4, 400}, {5, 300}};

    // Allocate memory using First Fit
    cout << "\nAllocating using First Fit:" << endl;
    for (auto& process : processes) firstFit(blocks, process);
    displayMemoryStatus(blocks);

    // Reset memory blocks and deallocate processes
    for (auto& process : processes) deallocateMemory(blocks, process);

    // Allocate memory using Best Fit
    cout << "\nAllocating using Best Fit:" << endl;
    for (auto& process : processes) bestFit(blocks, process);
    displayMemoryStatus(blocks);

    // Reset memory blocks and deallocate processes
    for (auto& process : processes) deallocateMemory(blocks, process);

    // Allocate memory using Worst Fit with retry mechanism
    cout << "\nAllocating using Worst Fit:" << endl;
    for (auto& process : processes) worstFit(blocks, process);
    displayMemoryStatus(blocks);

    return 0;
}
