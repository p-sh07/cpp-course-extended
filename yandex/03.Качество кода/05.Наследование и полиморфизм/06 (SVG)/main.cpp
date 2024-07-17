#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays) {
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
    return polyline;
}

void Polymorph() {
    using namespace std;
    {
        Document doc;
        doc.Render(cout);
        cout << endl;
    } {
        Document doc;
        doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
        doc.Render(cout);
        cout << endl;
    } {
        Document doc;
        doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
        doc.Add(Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetFontWeight("bold"s)
                .SetData("Hello C++"s));
        doc.Add(Polyline().AddPoint({20, 20}).AddPoint({10, 15}).AddPoint({0, 10}));
        doc.Render(cout);
        cout << endl;
    } {
        Document doc;
        doc.AddPtr(make_unique<Circle>(std::move(
                                            Circle().SetCenter({20, 20}).SetRadius(10))));
        doc.AddPtr(make_unique<Text>(std::move(
                                          Text()
                                          .SetPosition({35, 20})
                                          .SetOffset({0, 6})
                                          .SetData("Hello C++"s))));
        doc.AddPtr(make_unique<Polyline>(std::move(
                                              Polyline().AddPoint({20, 20}).AddPoint({10, 15}).AddPoint({0, 10}))));
        doc.Render(cout);
    }
    {
        Document doc;
        doc.Add(Text().SetData("Hello, <UserName>. Would you like some \"M&M\'s\"?"s));
        doc.Render(cout);
    }
}

int main() {
    Polymorph();
    return 0;
}


/*
 <?xml version="1.0" encoding="UTF-8" ?>
 <svg xmlns="http://www.w3.org/2000/svg" version="1.1">
 </svg>
 <?xml version="1.0" encoding="UTF-8" ?>
 <svg xmlns="http://www.w3.org/2000/svg" version="1.1">
 <circle cx="20" cy="20" r="10" />
 </svg>
 <?xml version="1.0" encoding="UTF-8" ?>
 <svg xmlns="http://www.w3.org/2000/svg" version="1.1">
 <circle cx="20" cy="20" r="10" />
 <text x="35" y="20" dx="0" dy="6" font-size="12" font-family="Verdana" font-weight="bold">Hello C++</text>
 <polyline points="20,20 10,15 0,10" />
 </svg>
 <?xml version="1.0" encoding="UTF-8" ?>
 <svg xmlns="http://www.w3.org/2000/svg" version="1.1">
 <circle cx="20" cy="20" r="10" />
 <text x="35" y="20" dx="0" dy="6" font-size="1">Hello C++</text>
 <polyline points="20,20 10,15 0,10" />
 </svg>
 */
