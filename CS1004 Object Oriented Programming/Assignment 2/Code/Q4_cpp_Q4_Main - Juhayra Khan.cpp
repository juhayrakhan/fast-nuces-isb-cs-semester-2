#include "Q4_Submission.h"
#include <iostream>
using namespace std;

void printMenu() {
    cout << "\n=== Whisper Audio Test Interface ===\n";
    cout << "1. Load WAV file\n";
    cout << "2. Activate track\n";
    cout << "3. Apply gain to active track\n";
    cout << "4. Reverse active track\n";
    cout << "5. Normalize active track\n";
    cout << "6. Apply filter to active track\n";
    cout << "7. Convert active track to spectrum\n";
    cout << "8. Splice active track\n";
    cout << "9. Combine active track with another\n";
    cout << "10. Print active track info\n";
    cout << "0. Exit\n";
    cout << "Choose option: ";
}

void printTrackInfo(Whisper* w) {
    if (!w) {
        cout << "No active track.\n";
        return;
    }
    cout << "Sample rate: " << w->header->sampleRate << "\n";
    cout << "Bit depth: " << w->header->bitsPerSample << "\n";
    cout << "Channels: " << w->header->numChannels << "\n";
    cout << "Sample count: " << w->sampleCount << "\n";
    cout << "First 10 samples: ";
    for (int i = 0; i < min(10, w->sampleCount); i++)
        cout << w->buffer[i] << " ";
    cout << "\n";
}

int main() {
    Orchestrator orch;
    while (true) {
        printMenu();
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            char* fname = new char[100];
            cout << "Enter WAV file path: ";
            cin.getline(fname, 100);
            if (orch.loadTrack(fname))
                cout << "Loaded track successfully at index " << orch.activeIndex << "\n";
            else
                cout << "Failed to load track. Make sure your wav file is in the same directory as this code.\n";
            break;
        }
        case 2: {
            int idx;
            cout << "Enter track index (0-3): ";
            cin >> idx;
            orch.activate(idx);
            cout << "Activated track " << idx << "\n";
            break;
        }
        case 3: {
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            float gain;
            cout << "Enter gain factor (e.g., 1.5): ";
            cin >> gain;
            orch.applyGain(gain);
            cout << "Applied gain.\n";
            break;
        }
        case 4:
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            orch.reverseActive();
            cout << "Reversed active track.\n";
            break;
        case 5:
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            orch.whispers[orch.activeIndex]->normalise();
            cout << "Normalized active track.\n";
            break;
        case 6: {
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            int ftype;
            cout << "Filter type: 1=Low, 2=High, 3=Band: ";
            cin >> ftype;
            float f1, f2;
            Filter* f = nullptr;
            int fftSize = orch.whispers[orch.activeIndex]->sampleCount;
            switch (ftype) {
            case 1:
                cout << "Enter cutoff frequency: ";
                cin >> f1;
                f = new Filter(fftSize);
                f->lowPass(f1, orch.whispers[orch.activeIndex]->header->sampleRate);
                break;
            case  2:
                cout << "Enter cutoff frequency: ";
                cin >> f1;
                f = new Filter(fftSize);
                f->highPass(f1, orch.whispers[orch.activeIndex]->header->sampleRate);
                break;
            case  3:
                cout << "Enter low cutoff: ";
                cin >> f1;
                cout << "Enter high cutoff: ";
                cin >> f2;
                f = new Filter(fftSize);
                f->bandPass(f1, f2, orch.whispers[orch.activeIndex]->header->sampleRate);
            }
            if (f) {
                orch.filterActive(f);
                cout << "Filter applied.\n";
                delete f;
            }
            break;
        }
        case 7:
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            orch.spectrumActive();
            cout << "Converted active track to spectrum.\n";
            break;
        case 8: {
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            double start, end;
            cout << "Enter start time (seconds): ";
            cin >> start;
            cout << "Enter end time (seconds): ";
            cin >> end;
            if (orch.activeIndex >= 0) {
                Whisper* sp = orch.whispers[orch.activeIndex]->splice(start, end);
                if (sp) {
                    delete orch.whispers[orch.activeIndex];
                    orch.whispers[orch.activeIndex] = sp;
                    cout << "Splice created.\n";
                }
                else cout << "Invalid splice.\n";
            }
            break;
        }
        case 9: {
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            int otherIdx;
            cout << "Enter index of track to combine with active: ";
            cin >> otherIdx;
            if (orch.activeIndex >= 0 && otherIdx >= 0 && otherIdx < 4 && orch.whispers[otherIdx])
                *orch.whispers[orch.activeIndex] += *orch.whispers[otherIdx];
            else cout << "Invalid track index.\n";
            break;
        }
        case 10:
            if (orch.activeIndex < 0) { cout << "No active track.\n"; break; }
            printTrackInfo(orch.whispers[orch.activeIndex]);
            break;
        case 0:
            return 0;
        default:
            cout << "Invalid option.\n";
        }
    }

    return 0;
}