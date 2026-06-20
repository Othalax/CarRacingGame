#include "Car.h"

Car::Car(std::unordered_map<std::string, sf::Keyboard::Key> keys, sf::Texture& texture, float x, float y, float angle, float length, float max_steering, float max_acceleration)
    : keys(keys), position(x, y), velocity(0.f, 0.f), angle(angle), length(length),
    max_acceleration(max_acceleration), max_steering(max_steering), max_velocity(100.f),
    brake_deceleration(20.f), free_deceleration(40.f), acceleration(0.f), steering(0.f)
{
    this->car = new sf::Sprite(texture);
    this->car->setOrigin({ 100.f, 50.f });
    this->car->setPosition({ x, y });
    this->car->scale({ 0.5f, 0.5f });
}

Car::~Car()
{
    delete this->car;
}

void Car::ride(const float& dt){
    velocity.x += acceleration * dt;
    velocity.x = std::clamp(velocity.x, -max_velocity, max_velocity);

    if (sf::Keyboard::isKeyPressed(keys["forward"])) {
        acceleration = std::min(acceleration + 50.0f * dt, max_acceleration);
    } else if (sf::Keyboard::isKeyPressed(keys["backward"])) {
        acceleration = std::max(acceleration - 50.0f * dt, -max_acceleration);
    } else {
        if (std::abs(velocity.x) > dt * free_deceleration)
            acceleration = -std::copysign(free_deceleration, velocity.x);
        else
            acceleration = -velocity.x / dt;
    }
}

void Car::veer(const float& dt){
    float angular_velocity = 0.0f;
    if (std::abs(steering) > 0.01f) {
        float turning_radius = length / std::tan(steering * PI / 180.0f);
        angular_velocity = velocity.x / turning_radius;
    }
    position.x += velocity.x * std::cos(angle * PI / 180.0f) * dt;
    position.y += velocity.x * std::sin(angle * PI / 180.0f) * dt;
    angle += angular_velocity * dt * (180.0f / PI);

     if (sf::Keyboard::isKeyPressed(keys["left"])) {
        steering = std::max(steering - 50 * dt, -max_steering);
    } else if (sf::Keyboard::isKeyPressed(keys["right"])) {
        steering = std::min(steering + 50 * dt, max_steering);
    } else {
        steering *= 0.8f;
    }
}

float Car::dotProduct(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

sf::Vector2f Car::normalizeVector(const sf::Vector2f& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    return (length != 0.f) ? v / length : sf::Vector2f(0.f, 0.f);
}

void Car::handleCollision(Car& other) {
    sf::FloatRect boundsThis = this->car->getLocalBounds();
    sf::Transform transThis = this->car->getTransform();

    sf::Vector2f vertsThis[4] = {
        transThis.transformPoint({boundsThis.position.x, boundsThis.position.y}),
        transThis.transformPoint({boundsThis.position.x + boundsThis.size.x, boundsThis.position.y}),
        transThis.transformPoint({boundsThis.position.x + boundsThis.size.x, boundsThis.position.y + boundsThis.size.y}),
        transThis.transformPoint({boundsThis.position.x, boundsThis.position.y + boundsThis.size.y})
    };

    sf::FloatRect boundsOther = other.car->getLocalBounds();
    sf::Transform transOther = other.car->getTransform();

    sf::Vector2f vertsOther[4] = {
        transOther.transformPoint({boundsOther.position.x, boundsOther.position.y}),
        transOther.transformPoint({boundsOther.position.x + boundsOther.size.x, boundsOther.position.y}),
        transOther.transformPoint({boundsOther.position.x + boundsOther.size.x, boundsOther.position.y + boundsOther.size.y}),
        transOther.transformPoint({boundsOther.position.x, boundsOther.position.y + boundsOther.size.y})
    };

    sf::Vector2f axes[4];
    axes[0] = normalizeVector(vertsThis[1] - vertsThis[0]); 
    axes[1] = sf::Vector2f(-axes[0].y, axes[0].x);        

    axes[2] = normalizeVector(vertsOther[1] - vertsOther[0]); 
    axes[3] = sf::Vector2f(-axes[2].y, axes[2].x);          

    float minOverlap = std::numeric_limits<float>::max();
    sf::Vector2f mtvAxis; 


    for (int i = 0; i < 4; ++i) {
        sf::Vector2f axis = axes[i];


        float minThis = dotProduct(vertsThis[0], axis);
        float maxThis = minThis;
        for (int j = 1; j < 4; ++j) {
            float proj = dotProduct(vertsThis[j], axis);
            minThis = std::min(minThis, proj);
            maxThis = std::max(maxThis, proj);
        }

        float minOther = dotProduct(vertsOther[0], axis);
        float maxOther = minOther;
        for (int j = 1; j < 4; ++j) {
            float proj = dotProduct(vertsOther[j], axis);
            minOther = std::min(minOther, proj);
            maxOther = std::max(maxOther, proj);
        }

        if (maxThis < minOther || maxOther < minThis) {
            return; 
        }

        float overlap = std::min(maxThis, maxOther) - std::max(minThis, minOther);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            mtvAxis = axis; 
        }
    }

    sf::Vector2f direction = this->position - other.position;
    if (dotProduct(direction, mtvAxis) < 0) {
        mtvAxis = -mtvAxis;
    }

    sf::Vector2f pushVector = mtvAxis * (minOverlap / 2.f);
    this->position += pushVector;
    other.position -= pushVector;

    this->velocity.x *= -0.2f;
    other.velocity.x *= -0.2f;
}

void Car::update(const float& dt) {

    ride(dt);
    veer(dt);

    this->car->setPosition(position);
    this->car->setRotation(sf::degrees(angle));
}

void Car::render(sf::RenderTarget& target){
    target.draw(*this->car);
}
