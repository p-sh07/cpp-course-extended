namespace shapes {

class Triangle : public svg::Drawable {
public:
    Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
    : p1_(p1)
    , p2_(p2)
    , p3_(p3)
    {}
    
    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
    }
    
private:
    svg::Point p1_, p2_, p3_;
};

class Star : public svg::Drawable {
public:
    
    Star(svg::Point center, double outer_rad, double inner_rad, int num_rays)
    : center_(center)
    , outer_rad_(outer_rad)
    , inner_rad_(inner_rad)
    , num_rays_(num_rays) {}
    
    void Draw(ObjectContainer& container) const override {
        container.Add(CreateStar(center_, outer_rad_, inner_rad_, num_rays_).SetFill("red").SetStroke("black"));
    }
    
    /* from sln:
     void Draw(svg::ObjectContainer& container) const override {
         using namespace std::literals;
         container.Add(  //
                 CreateStar(center_, outer_radius_, inner_radius_, num_rays_)
                         .SetFillColor("red"s)
                         .SetStrokeColor("black"s));
     }*/
    
private:
    svg::Point center_;
    double outer_rad_ = 0;
    double inner_rad_ = 0;
    int num_rays_ = 0;
    
    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) const {
        using namespace svg;
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
    
};

class Snowman : public svg::Drawable {
public:
    
    Snowman(svg::Point head_center, double head_rad)
    : head_center_(head_center)
    , head_rad_(head_rad) {}
    
    void Draw(ObjectContainer& container) const override {
        container.Add(ComputeBottom());
        container.Add(ComputeMid());
        container.Add(svg::Circle().SetCenter(head_center_).SetRadius(head_rad_).SetFill("rgb(240,240,240)").SetStroke("black"));
    }
    
    /* from sln:
     void Draw(svg::ObjectContainer& container) const override {
         using namespace svg;
         using namespace std::literals;

         const auto base_circle = Circle().SetFillColor("rgb(240,240,240)"s).SetStrokeColor("black");
         container.Add(  //
                 Circle(base_circle)
                         .SetCenter({head_center_.x, head_center_.y + 5 * head_radius_})
                         .SetRadius(2 * head_radius_));

         container.Add(  //
                 Circle(base_circle)
                         .SetCenter({head_center_.x, head_center_.y + 2 * head_radius_})
                         .SetRadius(1.5 * head_radius_));
         container.Add(Circle(base_circle).SetCenter(head_center_).SetRadius(head_radius_));
     }*/
    
private:
    svg::Point head_center_;
    double head_rad_;
    
    svg::Circle ComputeMid() const {
        return svg::Circle().SetCenter({head_center_.x, head_center_.y + 2 * head_rad_}).SetRadius(1.5 * head_rad_).SetFill("rgb(240,240,240)").SetStroke("black");
    }
    
    svg::Circle ComputeBottom() const {
        return svg::Circle().SetCenter({head_center_.x, head_center_.y + 5 * head_rad_}).SetRadius(2 * head_rad_).SetFill("rgb(240,240,240)").SetStroke("black");
    }
    
};

}
