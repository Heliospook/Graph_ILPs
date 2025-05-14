// Strategy : Packing idleness around jobs
 
#include <bits/stdc++.h>
using namespace std;
 
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
 
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
 
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
 
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};
 
#ifdef HELIOSPOOK
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif
 
typedef long long ll;
typedef long double ld;
#define int ll
typedef vector<int> vi;
typedef pair<int,int> pi;
#define pb push_back
#define all(v) v.begin(), v.end()
#define MAX 200000
#define MOD 1000000007
#define nl '\n'
const int inf = 1e17;
 
template <typename T>
void takeinput(vector<T> &a, int start, int end){
    for(int i = start; i <= end; i++) cin>>a[i];
}
void yes(){cout<<"YES"<<nl;}
void no(){cout<<"NO"<<nl;}
void testcase(int t){cout<<"Case #"<<t<<" : ";}
int exp(int a, int b){
    int res = 1;
    while(b > 0){
        if( b % 2 == 1) res = (res * a) % MOD;
        a = (a * a) % MOD; b /= 2;
    }
    return res;
}
int ceildiv(int a, int b){
    return (a + b - 1)/b;
}
void init(){
    return ;
}
 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct Interval{
    int l, r, i;
};
struct Event{
    int type, mnum, ind;
};
void __print(Interval x) {cerr << "("<<x.l<<", "<<x.r<<")";}
 
void solve(int t_){
    int n, y, x; cin>>n>>y>>x;
    vector<Interval> intervals;
    for(int i = 0; i < n; i++){
        int l, r; cin>>l>>r; intervals.pb({l, r, i});
    }

    sort(all(intervals), [](Interval a, Interval b)->bool{
        if(a.l == b.l){
            return a.r < b.r;
        }
        return a.l < b.l;
    });

    int mxtime = intervals.back().r;

    vector<Event> ans;
    auto check = [&](int m)->bool{
        ans.clear();
        vector<int> last(m, -1);
        int i = 0;
        for(int period = 0; period <= mxtime; period += y){
            int period_end = period + y - 1;
            vector<Interval> period_intervals;
            while(i < n && intervals[i].l <= period_end){
                period_intervals.push_back(intervals[i++]);
            }
            for(int j = 0; j < m; j++){
                last[j] = max(last[j], period - 1);
            }
            
            vector<int> done(m, false);
            for(auto interval : period_intervals){
                Interval inFront = {interval.l - x, interval.r, interval.i};
                Interval atBack = {interval.l, interval.r + x, interval.i};
                bool sched = false;
                for(int j = 0; j < m; j++){
                    // first try to schedule raw
                    if(done[j]){
                        if(last[j] < interval.l){
                            last[j] = max(last[j], interval.r); sched = true; 
                            ans.pb({1, j, interval.i});
                            break;
                        }
                    }else{
                        // Now try the inFront one
                        if(last[j] < inFront.l && last[j] + x <= period_end){
                            ans.pb({1, j, interval.i}); ans.pb({2, j, last[j] + 1});
                            last[j] = max(last[j], inFront.r); sched = true;
                            done[j] = true;
                            break;
                        }
                        // Finally the atBack one
                        if(last[j] < atBack.l && atBack.r <= period_end){
                            ans.pb({1, j, interval.i}); ans.pb({2, j, atBack.r - x + 1});
                            last[j] = max(last[j], atBack.r); sched = true;
                            done[j] = true;
                            break;
                        }
                    }
                }
                if(!sched){
                    return false;
                }
            }
            for(int j = 0; j < m; j++){
                if(done[j]) continue;
                if(last[j] + x - 1 > period_end){
                    return false;
                }
                ans.pb({2, j, last[j] + 1});
                last[j] = max(last[j], last[j] + x);
            }
        }
        return true;
    };


    int l = 1, r = n;
    int m = r;
    check(m);
    vector<Event> finalans(ans.begin(), ans.end());

    while(l <= r){
        int mid = (l + r)/2;
        if(check(mid)){
            m = mid; finalans = ans;
            r = mid - 1;
        }else l = mid + 1;
    }

    cout<<m<<nl;
    cout<<finalans.size()<<nl;
    for(auto ai : finalans){
        cout<<ai.type<<" "<<ai.mnum<<" "<<ai.ind<<nl;
    }
}
 
signed main(){
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    #ifdef HELIOSPOOK
    freopen("input.in","r",stdin);
    freopen("output.out","w",stdout);
    freopen("error.out","w",stderr);
    #endif
 
    init();
    int t = 1;
    cin>>t;
    for(int i=1;i<=t;i++){
        solve(i);
    }
}
