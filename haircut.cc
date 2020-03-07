// For the Riddler Classic at https://fivethirtyeight.com/features/can-you-get-a-haircut-already/
// Officially, but incorrectly, solved at https://fivethirtyeight.com/features/how-good-are-you-at-guess-who/
// The problem with the official solution is that it assumes the person who's waiting has a 1/4 chance of
// wanting Tiffany. But if you have five people walk into the shop, the one who's waiting has a much 
// higher chance of wanting Tiffany.
//
// In this code, we'll assume that:
// 1. The people in the shop are the first five customers of the day
// 2. They all came in at a random time between 0 and 15 minutes into the day
// 3. You came in 15 minutes into the day
// 4. Customers who don't request Tiffany are assigned the soonest available barber, breaking ties at random.

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>

using namespace std;

struct Result {
    bool wantedTiffany;
    double minutesWaited;

    Result() {
        wantedTiffany = false;
        minutesWaited = 0;
    }
};

Result simulate(bool debug) {
    vector<double> arrivalTimes;
    for (int i = 0; i < 5; i++) {
        arrivalTimes.push_back(rand() * 1.0 / RAND_MAX * 15);
    }
    sort(arrivalTimes.begin(), arrivalTimes.end());
    vector<double> nextDone(4, 0);

    Result result;
    for (int i = 0; i < 5; i++) {
        bool wantedTiffany = (rand() % 4) == 0;
        double arrivalTime = arrivalTimes[i];
        int barber = -1;
        if (wantedTiffany && nextDone[0] > 0) result.wantedTiffany = true;
        if (wantedTiffany) {
            barber = 0;
        } else if (i == 4) {
            // Pick the first available
            int soonest = 0;
            for (int j = 0; j < 4; j++) {
                if (nextDone[j] < nextDone[soonest]) soonest = j;
            }
            barber = soonest;
        } else {
            int numFree = 4 - i;
            int numChosen = rand() % numFree;
            for (int j = 0; j < 4; j++) {
                if (nextDone[j] == 0 && numChosen-- == 0) {
                    barber = j;
                    break;
                }
            }
        }
        nextDone[barber] = max(nextDone[barber] + 15, arrivalTime + 15);
        if (debug) cout << "Customer " << i << " " << (wantedTiffany ? "wanted Tiffany" : "didn't want Tiffany") << " and arrived at " << arrivalTime << ", got barber " << barber << " and will be done at " << nextDone[barber] << endl;
    }
    if (debug) cout << "result.wantedTiffany = " << result.wantedTiffany << endl;
    result.minutesWaited = nextDone[0];
    return result;
}

int main() {
    srand(time(NULL));

    vector<Result> results;
    for (int i = 0; i < 1000000; i++) {
        results.push_back(simulate(i < 20));
    }

    int wanted = 0;
    double minutes = 0.0;
    for (int i = 0; i < results.size(); i++) {
        if (results[i].wantedTiffany) ++wanted;
        minutes += results[i].minutesWaited;
    }
    minutes /= results.size();
    cout << wanted << "/" << results.size() << " (" << (wanted * 100.0 / results.size()) << "%) wanted Tiffany. Average wait time: " << (minutes - 15) << "minutes." << endl;
}
