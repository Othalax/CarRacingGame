#include "Car.h"

#include "Config.h"

#include <numbers>

Car::Car(std::unordered_map<std::string, sf::Keyboard::Key> keys, sf::Texture& texture)
    : keys(std::move(keys)), position(0.F, 0.F), speed(0.F), angle(0.F), acceleration(0.F),
      steering(0.F) {
    const auto& physics = Config::instance().get_car_physics();
    this->length = physics.length;
    this->max_acceleration = physics.maxAcceleration;
    this->max_steering = physics.maxSteering;
    this->max_velocity = physics.maxVelocity;
    this->brake_deceleration = physics.brakeDeceleration;
    this->free_deceleration = physics.freeDeceleration;

    this->car = std::make_unique<sf::Sprite>(texture);
    this->car->setOrigin(physics.spriteOrigin);
    this->car->setPosition(position);
    this->car->scale(physics.spriteScale);
}

void Car::set_position(sf::Vector2f position, float angle) {
    this->position = position;
    this->angle = angle;
}

void Car::ride(const float& dt) {
    const auto& physics = Config::instance().get_car_physics();

    speed += acceleration * dt;
    speed = std::clamp(speed, -max_velocity, max_velocity);

    if (sf::Keyboard::isKeyPressed(keys["forward"])) {
        acceleration = std::min(acceleration + (physics.accelerationRate * dt), max_acceleration);
    } else if (sf::Keyboard::isKeyPressed(keys["backward"])) {
        acceleration = std::max(acceleration - (physics.accelerationRate * dt), -max_acceleration);
    } else {
        if (std::abs(speed) > dt * free_deceleration) {
            acceleration = -std::copysign(free_deceleration, speed);
        } else {
            acceleration = -speed / dt;
        }
    }
}

void Car::veer(const float& dt) {
    const auto& physics = Config::instance().get_car_physics();
    float angular_velocity = 0.F;
    if (std::abs(steering) > 0.01F) {
        const float turning_radius =
            length / std::tan(steering * static_cast<float>(std::numbers::pi_v<float>) / 180.F);
        angular_velocity = speed / turning_radius;
    }
    position.x +=
        speed * std::cos(angle * static_cast<float>(std::numbers::pi_v<float>) / 180.F) * dt;
    position.y +=
        speed * std::sin(angle * static_cast<float>(std::numbers::pi_v<float>) / 180.F) * dt;
    angle += angular_velocity * dt * (180.F / static_cast<float>(std::numbers::pi_v<float>));

    if (sf::Keyboard::isKeyPressed(keys["left"])) {
        steering = std::max(steering - (physics.steeringRate * dt), -max_steering);
    } else if (sf::Keyboard::isKeyPressed(keys["right"])) {
        steering = std::min(steering + (physics.steeringRate * dt), max_steering);
    } else {
        steering *= physics.steeringDamping;
    }
}

float Car::dot_product(const sf::Vector2f& a, const sf::Vector2f& b) {
    return (a.x * b.x) + (a.y * b.y);
}

sf::Vector2f Car::normalize_vector(const sf::Vector2f& v) {
    const float length = std::sqrt((v.x * v.x) + (v.y * v.y));
    return (length != 0.F) ? v / length : sf::Vector2f(0.F, 0.F);
}

void Car::handle_collision(Car& other) {
    const float collision_speed_multiplier =
        Config::instance().get_car_physics().collisionSpeedMultiplier;

    const sf::FloatRect bounds_this = this->car->getLocalBounds();
    const sf::Transform trans_this = this->car->getTransform();

    const std::array<sf::Vector2f, 4> verts_this = {
        trans_this.transformPoint({bounds_this.position.x, bounds_this.position.y}),
        trans_this.transformPoint(
            {bounds_this.position.x + bounds_this.size.x, bounds_this.position.y}),
        trans_this.transformPoint({bounds_this.position.x + bounds_this.size.x,
                                   bounds_this.position.y + bounds_this.size.y}),
        trans_this.transformPoint(
            {bounds_this.position.x, bounds_this.position.y + bounds_this.size.y})};

    const sf::FloatRect bounds_other = other.car->getLocalBounds();
    const sf::Transform trans_other = other.car->getTransform();

    const std::array<sf::Vector2f, 4> verts_other = {
        trans_other.transformPoint({bounds_other.position.x, bounds_other.position.y}),
        trans_other.transformPoint(
            {bounds_other.position.x + bounds_other.size.x, bounds_other.position.y}),
        trans_other.transformPoint({bounds_other.position.x + bounds_other.size.x,
                                    bounds_other.position.y + bounds_other.size.y}),
        trans_other.transformPoint(
            {bounds_other.position.x, bounds_other.position.y + bounds_other.size.y})};

    std::array<sf::Vector2f, 4> axes;
    axes.at(0) = normalize_vector(verts_this.at(1) - verts_this.at(0));
    axes.at(1) = sf::Vector2f(-axes.at(0).y, axes.at(0).x);

    axes.at(2) = normalize_vector(verts_other.at(1) - verts_other.at(0));
    axes.at(3) = sf::Vector2f(-axes.at(2).y, axes.at(2).x);

    float min_overlap = std::numeric_limits<float>::max();
    sf::Vector2f mtv_axis;

    for (const auto& axis : axes) {

        float min_this = Car::dot_product(verts_this.front(), axis);
        float max_this = min_this;
        for (const auto& vert : verts_this) {
            const float proj = Car::dot_product(vert, axis);
            min_this = std::min(min_this, proj);
            max_this = std::max(max_this, proj);
        }

        float min_other = Car::dot_product(verts_other.front(), axis);
        float max_other = min_other;
        for (const auto& vert : verts_other) {
            const float proj = Car::dot_product(vert, axis);
            min_other = std::min(min_other, proj);
            max_other = std::max(max_other, proj);
        }

        if (max_this < min_other || max_other < min_this) {
            return;
        }

        const float overlap = std::min(max_this, max_other) - std::max(min_this, min_other);
        if (overlap < min_overlap) {
            min_overlap = overlap;
            mtv_axis = axis;
        }
    }

    const sf::Vector2f direction = this->position - other.position;
    if (Car::dot_product(direction, mtv_axis) < 0) {
        mtv_axis = -mtv_axis;
    }

    const sf::Vector2f push_vector = mtv_axis * (min_overlap / 2.F);
    this->position += push_vector;
    other.position -= push_vector;

    this->speed *= collision_speed_multiplier;
    other.speed *= collision_speed_multiplier;
}

void Car::handle_wall_collision(const std::vector<sf::Vector2f>& wallVertices) {
    const sf::FloatRect bounds_this = this->car->getLocalBounds();
    const sf::Transform trans_this = this->car->getTransform();

    std::vector<sf::Vector2f> verts_this = {
        trans_this.transformPoint({bounds_this.position.x, bounds_this.position.y}),
        trans_this.transformPoint(
            {bounds_this.position.x + bounds_this.size.x, bounds_this.position.y}),
        trans_this.transformPoint({bounds_this.position.x + bounds_this.size.x,
                                   bounds_this.position.y + bounds_this.size.y}),
        trans_this.transformPoint(
            {bounds_this.position.x, bounds_this.position.y + bounds_this.size.y})};

    std::vector<sf::Vector2f> axes;

    for (size_t i = 0; i < 4; ++i) {
        const sf::Vector2f p1 = verts_this.at(i);
        const sf::Vector2f p2 = verts_this.at((i + 1) % 4);
        const sf::Vector2f edge = p2 - p1;
        sf::Vector2f normal(-edge.y, edge.x);

        normal = normalize_vector(normal);
        axes.push_back(normal);
    }

    const size_t wall_count = wallVertices.size();
    for (size_t i = 0; i < wall_count; ++i) {
        const sf::Vector2f p1 = wallVertices.at(i);
        const sf::Vector2f p2 = wallVertices.at((i + 1) % wall_count);
        const sf::Vector2f edge = p2 - p1;
        sf::Vector2f normal(-edge.y, edge.x);

        normal = normalize_vector(normal);
        axes.push_back(normal);
    }

    float min_overlap = std::numeric_limits<float>::max();
    sf::Vector2f mtv_axis(0.F, 0.F);

    for (const auto& axis : axes) {
        float min_this = Car::dot_product(verts_this.front(), axis);
        float max_this = min_this;
        for (size_t j = 1; j < 4; ++j) {
            const float proj = Car::dot_product(verts_this.at(j), axis);
            min_this = std::min(min_this, proj);
            max_this = std::max(max_this, proj);
        }

        float min_wall = Car::dot_product(wallVertices.at(0), axis);
        float max_wall = min_wall;
        for (size_t j = 1; j < wall_count; ++j) {
            const float proj = Car::dot_product(wallVertices.at(j), axis);
            min_wall = std::min(min_wall, proj);
            max_wall = std::max(max_wall, proj);
        }

        if (max_this < min_wall || max_wall < min_this) {
            return;
        }

        const float overlap = std::min(max_this, max_wall) - std::max(min_this, min_wall);
        if (overlap < min_overlap) {
            min_overlap = overlap;
            mtv_axis = axis;
        }
    }

    const sf::Vector2f car_center = this->position;

    sf::Vector2f closest_vertex = wallVertices.at(0);
    float min_dist = std::numeric_limits<float>::max();

    for (const auto& v : wallVertices) {
        const float dist = std::pow(v.x - car_center.x, 2.F) + std::pow(v.y - car_center.y, 2.F);
        if (dist < min_dist) {
            min_dist = dist;
            closest_vertex = v;
        }
    }

    const sf::Vector2f direction = car_center - closest_vertex;

    if (Car::dot_product(direction, mtv_axis) < 0) {
        mtv_axis = -mtv_axis;
    }

    this->position += mtv_axis * min_overlap;
    this->speed = 0.F;
}

bool Car::check_winning(const std::vector<sf::Vector2f>& finishLine) {
    const sf::FloatRect bounds_this = this->car->getLocalBounds();
    const sf::Transform trans_this = this->car->getTransform();

    const std::array<sf::Vector2f, 4> verts_this = {
        trans_this.transformPoint({bounds_this.position.x, bounds_this.position.y}),
        trans_this.transformPoint(
            {bounds_this.position.x + bounds_this.size.x, bounds_this.position.y}),
        trans_this.transformPoint({bounds_this.position.x + bounds_this.size.x,
                                   bounds_this.position.y + bounds_this.size.y}),
        trans_this.transformPoint(
            {bounds_this.position.x, bounds_this.position.y + bounds_this.size.y})};

    std::array<sf::Vector2f, 4> axes;
    axes.at(0) = normalize_vector(verts_this.at(1) - verts_this.front());
    axes.at(1) = sf::Vector2f(-axes.at(0).y, axes.at(0).x);
    axes.at(2) = normalize_vector(finishLine.at(1) - finishLine.at(0));
    axes.at(3) = sf::Vector2f(-axes.at(2).y, axes.at(2).x);

    for (const auto& axis : axes) {

        float min_this = Car::dot_product(verts_this.front(), axis);
        float max_this = min_this;

        for (const auto& vert : verts_this) {
            const float proj = Car::dot_product(vert, axis);
            min_this = std::min(min_this, proj);
            max_this = std::max(max_this, proj);
        }

        float min_other = Car::dot_product(finishLine.front(), axis);
        float max_other = min_other;
        for (size_t j = 1; j < 4; ++j) {
            const float proj = Car::dot_product(finishLine.at(j), axis);
            min_other = std::min(min_other, proj);
            max_other = std::max(max_other, proj);
        }

        if (max_this < min_other || max_other < min_this) {
            return false;
        }
    }

    return true;
}

void Car::update(const float& dt) {

    ride(dt);
    veer(dt);

    this->car->setPosition(position);
    this->car->setRotation(sf::degrees(angle));
}

void Car::render(sf::RenderTarget& target) {
    target.draw(*this->car);
}
