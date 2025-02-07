#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct MemoryBlock {
    int size;
    int id;
    bool allocated;

    MemoryBlock(int id, int size) : id(id), size(size), allocated(false) {}
};

struct Process {
    int id;
    int size;
    int allocatedBlock;

    Process(int id, int size) : id(id), size(size), allocatedBlock(-1) {}
};

// Function to allocate memory using First Fit with Splitting
void firstFitWithSplitting(vector<MemoryBlock>& blocks, Process& process) {
    for (int i = 0; i < blocks.size(); i++) {
        if (!blocks[i].allocated && blocks[i].size >= process.size) {
            int remainingSize = blocks[i].size - process.size;

            // Split the block if there is leftover space
            if (remainingSize > 0) {
                blocks[i].size = process.size; // Allocate exact size
                blocks.insert(blocks.begin() + i + 1, MemoryBlock(blocks.size() + 1, remainingSize)); // New block
            }

            blocks[i].allocated = true;
            process.allocatedBlock = blocks[i].id;
            cout << "Process " << process.id << " allocated to block " << blocks[i].id << " (Size: " << process.size << ")\n";
            return;
        }
    }
    cout << "Process " << process.id << " cannot be allocated (First Fit with Splitting)\n";
}

// Function to deallocate memory
void deallocate(vector<MemoryBlock>& blocks, Process& process) {
    if (process.allocatedBlock == -1) {
        cout << "Process " << process.id << " is not allocated, so cannot be deallocated.\n";
        return;
    }

    for (auto& block : blocks) {
        if (block.id == process.allocatedBlock) {
            block.allocated = false;  // Free the block
            process.allocatedBlock = -1;  // Reset process allocation
            cout << "Process " << process.id << " deallocated from block " << block.id << " (Size: " << block.size << ")\n";
            return;
        }
    }
}

// Function to compact memory (Merging free blocks)
void compactMemory(vector<MemoryBlock>& blocks) {
    vector<MemoryBlock> newBlocks;
    int totalFreeSize = 0;

    // Collect all allocated blocks and count total free memory
    for (const auto& block : blocks) {
        if (block.allocated) {
            newBlocks.push_back(block);  // Keep allocated blocks
        } else {
            totalFreeSize += block.size; // Sum free space
        }
    }

    // If free memory exists, add a single large free block
    if (totalFreeSize > 0) {
        newBlocks.push_back(MemoryBlock(newBlocks.size() + 1, totalFreeSize));
    }

    // Reassign proper sequential block IDs
    for (int i = 0; i < newBlocks.size(); i++) {
        newBlocks[i].id = i + 1;
    }

    blocks = newBlocks;
    cout << "\nMemory Compacted! Free memory combined into one block.\n";
}

// Function to display memory status
void displayMemoryStatus(const vector<MemoryBlock>& blocks) {
    cout << "\nMemory Blocks Status:\n";
    for (const auto& block : blocks) {
        cout << "Block " << block.id << ": Size = " << block.size
             << (block.allocated ? ", Allocated" : ", Not Allocated") << endl;
    }
    cout << endl;
}

int main() {
    vector<MemoryBlock> blocks = {
        {1, 500}, {2, 300}, {3, 200}, {4, 100}
    };

    vector<Process> processes = {
        {1, 200}, {2, 100}, {3, 250}, {4, 150}
    };

    // Allocate memory using First Fit with Splitting
    cout << "\nAllocating using First Fit with Splitting:\n";
    for (auto& process : processes) {
        firstFitWithSplitting(blocks, process);
    }
    displayMemoryStatus(blocks);

    // Deallocate some processes
    cout << "\nDeallocating some processes:\n";
    deallocate(blocks, processes[1]);  // Deallocate Process 2
    displayMemoryStatus(blocks);

    // Apply Memory Compaction
    compactMemory(blocks);
    displayMemoryStatus(blocks);

    return 0;
}
