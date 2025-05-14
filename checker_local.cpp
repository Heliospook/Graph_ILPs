#include "testlib.h"
#include <vector>
#include <utility>
#include <map> 
#include <set>
#include <algorithm>
using namespace std;

struct Interval{
    int l, r, i;
};

struct Event{
    int type, mnum, ind;
};

int check(InStream &inf, InStream &ouf, InStream &ans, string &message, int tc){
    message = "TC : ";
    message += toString(tc);
    message += ", ";

    int n = inf.readInt(), y = inf.readInt(), x = inf.readInt();
    vector<Interval> intervals;
    for(int i = 0; i < n; i++){
        int l = inf.readInt(), r = inf.readInt();
        intervals.push_back({l, r, i});
    }
    
    int m = ouf.readInt();
    int b = ouf.readInt();
    vector<Event> events;
    for(int i = 0; i < b; i++){
        int type = ouf.readInt(), mnum = ouf.readInt(), ind = ouf.readInt();
        events.push_back({type, mnum, ind});
    }

    int ansm = ans.readInt();
    int ansb = ans.readInt();
    for(int i = 0; i < ansb; i++){
        int type = ans.readInt(), mnum = ans.readInt(), ind = ans.readInt();
    }
    // Checks
    
    // 1. All intervals are scheduled
    map<int, vector<Interval>> intermp;
    map<int, vector<Interval>> restmp;
    map<int, int> lasttime;

    set<int> covered;
    for(auto event : events){
        if(event.type == 1){
            int ind = event.ind;
            if(ind >= n || ind < 0){
                message += "Interval index out of range, ";
                message += toString(ind);
                return -1;
            }
            if(event.mnum >= m || event.mnum < 0){
                message += "Invalid machine number";
                return -1;
            }
            covered.insert(ind);
            intermp[event.mnum].push_back(intervals[ind]);
            lasttime[event.mnum] = max(lasttime[event.mnum], intervals[ind].r);
        }else{
            int start_time = event.ind;
            if(event.mnum >= m || event.mnum < 0){
                message += "Invalid machine number";
                return -1;
            }
            intermp[event.mnum].push_back({start_time, start_time + x - 1, n});
            restmp[event.mnum].push_back({start_time, start_time + x - 1, n});
        }
    }
    int cov_size = covered.size();
    if(cov_size < n){
        message += "All intervals not scheduled.";
        return -1;
    }

    // 2. No overlaps
    for(auto [mnum, mintervals] : intermp){
        sort(mintervals.begin(), mintervals.end(), [](Interval a, Interval b)->bool{
            if(a.l == b.l) return a.r < b.r;
            return a.l < b.l;
        });
        int last = -1;
        for(auto interval : mintervals){
            if(last >= interval.l){
                message += "Overlapping intervals on a single machine. Machine num : ";
                message += toString(mnum);
                message += ", Interval : (" + toString(interval.l) + ", " + toString(interval.r) + ") , last : " + toString(last);
                return -1;
            }
            last = max(last, interval.r);
        }
    }

    // 3. Each machine should get enough idle-time
    for(auto [mnum, mintervals] : restmp){
        set<int> covered_periods;
        for(auto interval : mintervals){
            covered_periods.insert((interval.l / y) * y);
        }
        int curr = 0;
        while(curr < lasttime[mnum]){
            if(covered_periods.find(curr) == covered_periods.end()){
                message += "Idle-time not allocated properly. Machine num : ";
                message += toString(mnum);
                message += ". Period : ";
                message += toString(curr);
                return -1;
            }
            curr += y;
        }
    }
    cerr<<(n - m)<<endl;
    return n - m;
}

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);
    // inf - stream with the testdata.
    // ouf - stream with the contestant output.
    // ans - stream with the jury answer.
    int t = inf.readInt();
    bool wa = false;
    string message;
    int score = 0;

    for(int i = 1; i <= t; i++){
        string message_temp;
        int result = check(inf, ouf, ans, message_temp, i);
        if(result < 0){
            message = message_temp; wa = true;
        }else{
            score += result;
        }
    }
    
    if(wa){
        quitf(_wa, message.c_str());
    }else{
        double d_score = score;
        quitp(d_score, "OK");
    }
}