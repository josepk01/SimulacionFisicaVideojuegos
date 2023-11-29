class ForceGenerator {
protected:
    std::string _name;

public:
    ForceGenerator(std::string name) : _name(name) {}

    virtual void updateForce(Particle* particle, float duration) = 0;

    std::string getName() const {
        return _name;
    }
};
