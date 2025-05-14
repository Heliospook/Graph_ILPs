#include<bits/stdc++.h>
using namespace std;


#define int int64_t
#define ll long long
#define nl "\n"
#define FOR(i,b,c) for (int i = b; i < c; i++)
 
const int32_t MAXN = 4e6+1;
const int32_t eps = 1e-6;
const int N = 2e6+5;
const int M = 1e6;
const int MOD = 998244353;

struct machine {
    int id;
    vector<array<int,3>> intervals;
    int added = 0;
    machine(int a) {
        id = a;
    }
};

class scheduling {
public:
    vector<machine> v;
    int x, y;
    scheduling(int x_, int y_) {
        v.push_back(machine(0));
        v[0].added++;
        v[0].intervals.push_back({-1,0,-1});
        x = x_, y = y_;
    }

    void schedule(int l, int r, int id) {
        int cnt = (r+y-1)/y;
        for (int i = 0; i < int(v.size()); i++) {
            int added = v[i].added;
            while (v[i].added < cnt) {
                v[i].intervals.push_back({-1,v[i].added*y,-1});
                v[i].added++;
            }
        }
        bool inserted = false;
        // debug(v.size(),v.back().intervals);
        for (int i = 0; i < int(v.size()); i++) {
            // debug(i);s
            int valid = 2;
            int y11 = -1;
            int ind1 = -1;
            int foundind = -1;
            int cnt2 = 0;
            for (auto [x1,y1,id1] : v[i].intervals) {
                ind1++;
                if (x1 == -1) {
                    int x111 = y1;
                    int y111 = y1+x-1;
                    if (y111 < l) continue;
                    if (r < x111) continue;
                    valid = 1;
                    y11 = y1;
                    foundind = ind1;
                    cnt2++;
                    // debug(x111,y111,l,r);
                    continue;
                }
                if (y1 < l) continue;
                if (r < x1) continue;
                valid = 0;
                break;
            }
            // debug(l,r,v[i].intervals,cnt2);
            if (cnt2 > 1) {
                valid = 0;
            }
            if (valid == 0) continue;
            if (valid == 1) {
                int l_ = y11, r_ = y11+x-1;
                int block = (r_+y-1)/y;
                // debug(i,l_,r_,r+1,r+x,block,foundind);
                if (schedule2(i,l_,r_,r+1,r+x,block,foundind)) {
                    // debug(i,v.size(),v[i].intervals,l,r,id);
                    v[i].intervals.push_back({l,r,id});
                    inserted = true;
                    break;
                }
            }
            else {
                v[i].intervals.push_back({l,r,id});
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            // debug(l,r,id,v.back().intervals);
            v.push_back(machine(0));
            v.back().added++;
            v.back().intervals.push_back({-1,0});
            while (v.back().added < cnt) {
                v.back().intervals.push_back({-1,v.back().added*y,-1});
                v.back().added++;
            }
            schedule(l,r,id);
        }
    }

    bool schedule2(int ind, int l, int r, int newl, int newr, int block, int foundind) {
        // for (int i = 0; i < int(v[ind].intervals.size()); i++) {
        //     if (v[ind].intervals[i][0] == -1 && v[ind].intervals[i][1] == l) {
        //         foundind = i;
        //         break;
        //     }
        // }
        // debug(ind,l,r,v[ind].intervals,foundind);
        if ((newr+y-1)/y != block) {
            return false;
        }
        // while (v[ind].added < block) {
        //     v[ind].intervals.push_back({-1,v[ind].added*y,-1});
        //     v[ind].added++;
        // }
        bool valid = true;
        for (auto [x1,y1,id1] : v[ind].intervals) {
            if (x1 == -1) {

                continue;
            }
            if (y1 < newl) continue;
            if (newr < x1) continue;
            valid = false;
            break;
        }
        if (valid) {
            v[ind].intervals.erase(v[ind].intervals.begin()+foundind);
            v[ind].intervals.push_back({-1,newl,-1});
            return true;
        }
        return false;
    }
};

int runcases(int qq)
{  
    
    int n,y,x;
    cin >> n >> y >> x;

    vector<array<int,3>> vp;
    for (int i = 0; i < n; i++) {
        int l,r;
        cin >> l >> r;
        vp.push_back({l,r,i});
    } 

    sort(vp.begin(),vp.end(),[] (auto a, auto b) -> bool {
        return a[1] < b[1];
    });

    scheduling sc(x,y);
    assert(x != 0);
    int id = 0;
    for (auto [l,r,i] : vp) {
        sc.schedule(l,r,i);
    }

    int m = sc.v.size();
    int b = 0;
    for (auto x : sc.v) {
        b += int(x.intervals.size());
    }

    cout << m << nl;
    cout << b << nl;

    for (int i = 0; i < m; i++) {
        for (auto [l,r,id] : sc.v[i].intervals) {
            if (l == -1) {
                cout << 2 << " " << i << " " << r << nl;
            }
            else {
                cout << 1 << " " << i << " " << id << nl;
            }
        }
    }

    return 0;
    
    
}

 
signed main()
{
    
    //..........Fast I/O.........//
    ios_base::sync_with_stdio(false);
    cin.tie(0);cout.tie(0);
    //..........................//

    #ifdef HELIOSPOOK
    freopen("input.in","r",stdin);
    freopen("output.out","w",stdout);
    freopen("error.out","w",stderr);
    #endif

    int t;
    cin >> t;

    for (int i = 1; i <= t; i ++) {
        runcases(i);
    }
   
    return 0;
 
}