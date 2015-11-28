/* 1884. Way to the University - http://acm.timus.ru/problem.aspx?num=1884
 *
 * Strategy:
 * Create a set of time segments, each representing the time period that a car would hit Egor.
 * Go through the ordered list of the endpoints and choose the time of the first gap as answer.
 *
 * Performance:
 * O((m+n) log (m+n), runs in 0.015s using 556KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

class Fraction // For precision
{
public:
    long long num, den;

    Fraction(long long a, long long b) : num(a), den(b) { }

    Fraction(long long a) : num(a), den(1) { }

    Fraction() : num(0), den(1) { }

    Fraction operator+ (const Fraction& a) const
    {
        return Fraction(num*a.den + den*a.num, a.den*den);
    }

    Fraction operator- (const Fraction& a) const
    {
        return Fraction(num*a.den - den*a.num, a.den*den);
    }

    Fraction operator/ (const Fraction& a) const
    {
        return Fraction(num*a.den, a.num*den);
    }

    bool operator< (const Fraction&a) const
    {
        Fraction c = a-*this;
        return c.num*c.den > 0;
    }

    bool operator>= (const Fraction&a) const
    {
        Fraction c = *this-a;
        return c.num*c.den >= 0;
    }

    bool operator== (const Fraction& a) const
    {
        Fraction c = *this-a;
        return c.num*c.den == 0;
    }
};

struct Event
{
    bool start;
    Fraction t;
    bool operator() (const Event&a, const Event& b)
    {
        if(a.t == b.t)
            // If a start point and an end point are on the same point, we make sure to handle the
            // end point first (cars are apparently considered open sets, so tangenting the corner
            // counts as a miss)
            return !a.start && b.start; 
        else
            return a.t < b.t;
    }
};

// Reads n cars, farthest point at distance h into the street
void populate(std::vector<Event>& v, int n, int h)
{
    const Fraction vc(50, 9); // Speed of car, converted to m/s
    const Fraction ve(25, 18); //    ''   Egor
    while(n--)
    {
        int d;
        std::cin >> d;
        // First time point is for the farthest corner of the front of the car
        Fraction t1 = Fraction(d)/vc - Fraction(h)/ve;
        // Second time point is for the closest corner of the back of the car
        Fraction t2 = (Fraction(d) + 5)/vc - (Fraction(h)-2)/ve;
        v.push_back(Event { true, t1 } );
        v.push_back(Event { false, t2 });
    }
}

int main()
{
    int n, m;
    std::vector<Event> v;
    std::cin >> n;
    populate(v, n, 2);
    std::cin >> m;
    populate(v, m, 4);

    // Sentinel points to detect if it's safe to cross at time 0
    v.push_back( { true, Fraction(-1, 1) } );
    v.push_back( { false, Fraction(0, 1) } );
    std::sort(v.begin(), v.end(), Event());

    int h = 0; // Number of overlapping fatal time segments
    for(int i = 0; i < v.size(); i++)
    {
        if(!v[i].start)
        {
            h--;
            if(!h)
            {
                if(v[i].t >= 0)
                {
                    std::cout << std::fixed << std::setprecision(10) 
                              << (v[i].t.num)/double(v[i].t.den) << std::endl;
                    return 0;
                }
            }
        }
        else
            h++;
    }
    return 0;
}