#include "cenemy.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

cenemy::cenemy()
    : _posX(0.f), _posY(0.f), _health(3), _speed(3),
    _currentTarget(1), _reachedEnd(false), _pathLength(0),
    mRewardGiven(false),
    mReward(0),
    _isDead(false),
    _isAttack(false)
{
    // Movement direction offsets for BFS pathfinding (up, left, down, right)
    dd[0] = -1; dd[1] = 0; dd[2] = 1; dd[3] = 0;
    dc[0] = 0; dc[1] = -1; dc[2] = 0; dc[3] = 1;

    // Default grid positions
    _start = cpoint();
    _end = cpoint();
    _curr = cpoint();

    // Initialize path array with default points
    for (int i = 0; i < cpoint::MAP_ROW * cpoint::MAP_COL; i++)
        _p[i] = cpoint();
}

cenemy::cenemy(cpoint tstart, cpoint tend, cpoint tcurr) : cenemy() {
    _start = tstart;
    _end = tend;
    _curr = tcurr;
}

// Pathfinding core (BFS)
void cenemy::calcPath(int a[][cpoint::MAP_COL], int n, cpoint s, cpoint e) {
    std::queue<cpoint> q;
    bool visited[cpoint::MAP_ROW][cpoint::MAP_COL] = {};
    cpoint parent[cpoint::MAP_ROW][cpoint::MAP_COL];

    q.push(s);
    visited[s.getRow()][s.getCol()] = true;
    parent[s.getRow()][s.getCol()] = s;

    bool found = false;

    // BFS search
    while (!q.empty()) {
        cpoint curr = q.front(); q.pop();

        // If we reached the end, stop search
        if (curr.getRow() == e.getRow() && curr.getCol() == e.getCol()) {
            found = true;
            break;
        }

        // Check 4 neighbor cells
        for (int i = 0; i < 4; i++) {
            int dmoi = dd[i] + curr.getRow(), cmoi = dc[i] + curr.getCol();

            // Valid cell & walkable (C value = 0)
            if (dmoi >= 0 && dmoi < cpoint::MAP_ROW && cmoi >= 0 && cmoi < cpoint::MAP_COL
                && !visited[dmoi][cmoi] && a[dmoi][cmoi] == 0) {
                visited[dmoi][cmoi] = true;
                parent[dmoi][cmoi] = curr;
                q.push(cpoint(dmoi, cmoi, 0));
            }
        }
    }

    // If a path was found, reconstruct from end to start
    if (found) {
        std::vector<cpoint> pathList;
        cpoint curr = e;
        while (!(curr.getRow() == s.getRow() && curr.getCol() == s.getCol())) {
            pathList.push_back(curr);
            curr = parent[curr.getRow()][curr.getCol()];
        }
        pathList.push_back(s);

        // Reverse path to start → end order
        std::reverse(pathList.begin(), pathList.end());

        // Store into _p
        int i = 0;
        for (auto& p : pathList) _p[i++] = p;
        _pathLength = i;
    }
    else
        _pathLength = 0;
}

// Converts map from cpoint grid to integer grid & calls BFS
void cenemy::findPath(cpoint a[][cpoint::MAP_COL], cpoint s, cpoint e) {
    int ta[cpoint::MAP_ROW][cpoint::MAP_COL];
    for (int i = 0; i < cpoint::MAP_ROW; i++)
        for (int j = 0; j < cpoint::MAP_COL; j++)
            ta[i][j] = a[i][j].getC();
    calcPath(ta, cpoint::MAP_ROW, s, e);
}

void cenemy::updateSprite() {
    _sprite.setPosition(_posX, _posY);
}

void cenemy::setPosition(float x, float y) {
    _posX = x;
    _posY = y;
    updateSprite();
}

void cenemy::move(float dx, float dy) {
    _posX += dx;
    _posY += dy;
    updateSprite();
}

void cenemy::takeDamage(int damage) {
    if (_state == DEATH) return;

    _health -= damage;
    if (_health <= 0)
        startDeath();
}

void cenemy::init(EnemyType type, float x, float y, int hp, const EnemyAnimationData& data) {
    _posX = x; _posY = y;
    _health = hp;
    _type = type;

    mDamage = getDamageByType(type);
    mHasDamagedTower = false;

    loadFromData(data);

    _isDead = false;
    _reachedEnd = false;
    _isAttack = false;
    mReward = getResourcesByType(type);

    _sprite.setPosition(_posX, _posY);

    startWalk();
}

void cenemy::loadFromData(const EnemyAnimationData& data) {
    _walkTex = data.walkTex;
    _attackTex = data.attackTex;
    _deathTex = data.deathTex;

    _walkFrames = data.walkFrames;
    _attackFrames = data.attackFrames;
    _deathFrames = data.deathFrames;

    _walkSpeed = data.walkSpeed;
    _attackSpeed = data.attackSpeed;
    _deathSpeed = data.deathSpeed;

    _walkFrameWidth = data.walkFrameWidth;
    _walkFrameHeight = data.walkFrameHeight;
    _attackFrameWidth = data.attackFrameWidth;
    _attackFrameHeight = data.attackFrameHeight;
    _deathFrameWidth = data.deathFrameWidth;
    _deathFrameHeight = data.deathFrameHeight;

    _sprite.setScale(data.scaleX, data.scaleY); // Because the sizes of the sprite sheets are not the same
}

void cenemy::startWalk() {
    _sprite.setTexture(*_walkTex);
    _state = WALK;
    _anim.init(_walkFrameWidth, _walkFrameHeight, _walkSpeed, _walkFrames, /*loop*/true);
    refreshOriginByCurrentFrames(_anim.getFrameWidth(), _anim.getFrameHeight());
    _anim.applyTo(_sprite); // set initial rect
}

void cenemy::startAttack() {
    _sprite.setTexture(*_attackTex);
    _state = ATTACK;
    _isAttack = false; // will flip true when finished
    _anim.init(_attackFrameWidth, _attackFrameHeight, _attackSpeed, _attackFrames, /*loop*/false);
    refreshOriginByCurrentFrames(_anim.getFrameWidth(), _anim.getFrameHeight());
    _anim.applyTo(_sprite);
}

void cenemy::startDeath() {
    _sprite.setTexture(*_deathTex);
    _state = DEATH;
    _isDead = false; // will flip true when finished
    _anim.init(_deathFrameWidth, _deathFrameHeight, _deathSpeed, _deathFrames, /*loop*/false);
    refreshOriginByCurrentFrames(_anim.getFrameWidth(), _anim.getFrameHeight());
    _anim.applyTo(_sprite);
}

void cenemy::refreshOriginByCurrentFrames(int fw, int fh)
{
    _sprite.setOrigin(fw / 2.f, fh / 1.25f);
}

void cenemy::triggerAttack() {
    if (_state != WALK) return;
    startAttack();
}

void cenemy::updateAnimation(float deltaTime) {
    // If death already finalized, nothing to animate
    if (_state == DEATH && _isDead) return;

    _anim.update(deltaTime);
    _anim.applyTo(_sprite);

    if (_state == DEATH && _anim.isFinished())
        _isDead = true;

    else if (_state == ATTACK && _anim.isFinished())
        _isAttack = true;
}

void cenemy::faceLeft(EnemyType type) {
    if (type == RANGED_MECH)
        _sprite.setScale(-1.f, 1.f);
    else
        _sprite.setScale(-0.5f, 0.5f);
}

void cenemy::faceRight(EnemyType type) {
    if (type == RANGED_MECH)
        _sprite.setScale(1.f, 1.f);
    else
        _sprite.setScale(0.5f, 0.5f);
}

int cenemy::getHealthByType(EnemyType type) {
    switch (type) {
    case FAST_SCOUT: return 3;
    case RANGED_MECH: return 5;
    case HEAVY_WALKER: return 10;
    default: return 3;
    }
}

int cenemy::getSpeedByType(EnemyType type) {
    switch (type) {
    case FAST_SCOUT: return 120;
    case RANGED_MECH: return 100;
    case HEAVY_WALKER: return 70;
    default: return 70;
    }
}

int cenemy::getResourcesByType(EnemyType type) {
    switch (type) {
    case FAST_SCOUT: return 20;
    case RANGED_MECH: return 30;
    case HEAVY_WALKER: return 40;
    default: return 20;
    }
}

int cenemy::getDamageByType(EnemyType type)
{
    switch (type) {
    case FAST_SCOUT: return 1;
    case RANGED_MECH: return 2;
    case HEAVY_WALKER: return 3;
    default: return 100;
    }
}


#include "cenemy.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

cenemy::cenemy()
    : _posX(0.f), _posY(0.f), _health(3), _speed(3),
    _currentTarget(1), _reachedEnd(false), _pathLength(0),
    mRewardGiven(false),
    mReward(0),
    _isDead(false),
    _isAttack(false)
{
    // Movement direction offsets for BFS pathfinding (up, left, down, right)
    dd[0] = -1; dd[1] = 0; dd[2] = 1; dd[3] = 0;
    dc[0] = 0; dc[1] = -1; dc[2] = 0; dc[3] = 1;

    // Default grid positions
    _start = cpoint();
    _end = cpoint();
    _curr = cpoint();

    // Initialize path array with default points
    for (int i = 0; i < cpoint::MAP_ROW * cpoint::MAP_COL; i++)
        _p[i] = cpoint();
}

cenemy::cenemy(cpoint tstart, cpoint tend, cpoint tcurr) : cenemy() {
    _start = tstart;
    _end = tend;
    _curr = tcurr;
}

// Pathfinding core (BFS)
void cenemy::calcPath(int a[][cpoint::MAP_COL], int n, cpoint s, cpoint e) {
    std::queue<cpoint> q;
    bool visited[cpoint::MAP_ROW][cpoint::MAP_COL] = {};
    cpoint parent[cpoint::MAP_ROW][cpoint::MAP_COL];

    q.push(s);
    visited[s.getRow()][s.getCol()] = true;
    parent[s.getRow()][s.getCol()] = s;

    bool found = false;

    // BFS search
    while (!q.empty()) {
        cpoint curr = q.front(); q.pop();

        // If we reached the end, stop search
        if (curr.getRow() == e.getRow() && curr.getCol() == e.getCol()) {
            found = true;
            break;
        }

        // Check 4 neighbor cells
        for (int i = 0; i < 4; i++) {
            int dmoi = dd[i] + curr.getRow(), cmoi = dc[i] + curr.getCol();

            // Valid cell & walkable (C value = 0)
            if (dmoi >= 0 && dmoi < cpoint::MAP_ROW && cmoi >= 0 && cmoi < cpoint::MAP_COL
                && !visited[dmoi][cmoi] && a[dmoi][cmoi] == 0) {
                visited[dmoi][cmoi] = true;
                parent[dmoi][cmoi] = curr;
                q.push(cpoint(dmoi, cmoi, 0));
            }
        }
    }

    // If a path was found, reconstruct from end to start
    if (found) {
        std::vector<cpoint> pathList;
        cpoint curr = e;
        while (!(curr.getRow() == s.getRow() && curr.getCol() == s.getCol())) {
            pathList.push_back(curr);
            curr = parent[curr.getRow()][curr.getCol()];
        }
        pathList.push_back(s);

        // Reverse path to start → end order
        std::reverse(pathList.begin(), pathList.end());

        // Store into _p
        int i = 0;
        for (auto& p : pathList) _p[i++] = p;
        _pathLength = i;
    }
    else
        _pathLength = 0;
}

// Converts map from cpoint grid to integer grid & calls BFS
void cenemy::findPath(cpoint a[][cpoint::MAP_COL], cpoint s, cpoint e) {
    int ta[cpoint::MAP_ROW][cpoint::MAP_COL];
    for (int i = 0; i < cpoint::MAP_ROW; i++)
        for (int j = 0; j < cpoint::MAP_COL; j++)
            ta[i][j] = a[i][j].getC();
    calcPath(ta, cpoint::MAP_ROW, s, e);
}

void cenemy::updateSprite() {
    _sprite.setPosition(_posX, _posY);
}

void cenemy::setPosition(float x, float y) {
    _posX = x;
    _posY = y;
    updateSprite();
}

void cenemy::move(float dx, float dy) {
    _posX += dx;
    _posY += dy;
    updateSprite();
}

void cenemy::takeDamage(int damage) {
    if (_state == DEATH) return;

    _health -= damage;
    if (_health <= 0)
        startDeath();
}

void cenemy::init(EnemyType type, float x, float y, int hp, const EnemyAnimationData& data) {
    _posX = x; _posY = y;
    _health = hp;
    _type = type;

    mDamage = getDamageByType(type);
    mHasDamagedTower = false;

    loadFromData(data);

    _isDead = false;
    _reachedEnd = false;
    _isAttack = false;
    mReward = getResourcesByType(type);

    _sprite.setPosition(_posX, _posY);

    startWalk();
}

void cenemy::loadFromData(const EnemyAnimationData& data) {
    _walkTex = data.walkTex;
    _attackTex = data.attackTex;
    _deathTex = data.deathTex;

    _walkFrames = data.walkFrames;
    _attackFrames = data.attackFrames;
    _deathFrames = data.deathFrames;

    _walkSpeed = data.walkSpeed;
    _attackSpeed = data.attackSpeed;
    _deathSpeed = data.deathSpeed;

    _walkFrameWidth = data.walkFrameWidth;
    _walkFrameHeight = data.walkFrameHeight;
    _attackFrameWidth = data.attackFrameWidth;
    _attackFrameHeight = data.attackFrameHeight;
    _deathFrameWidth = data.deathFrameWidth;
    _deathFrameHeight = data.deathFrameHeight;

    _sprite.setScale(data.scaleX, data.scaleY); // Because the sizes of the sprite sheets are not the same
}

void cenemy::startWalk() {
    _sprite.setTexture(*_walkTex);
    _state = WALK;
    _anim.init(_walkFrameWidth, _walkFrameHeight, _walkSpeed, _walkFrames, /*loop*/true);
    refreshOriginByCurrentFrames(_anim.getFrameWidth(), _anim.getFrameHeight());
    _anim.applyTo(_sprite); // set initial rect
}

void cenemy::startAttack() {
    _sprite.setTexture(*_attackTex);
    _state = ATTACK;
    _isAttack = false; // will flip true when finished
    _anim.init(_attackFrameWidth, _attackFrameHeight, _attackSpeed, _attackFrames, /*loop*/false);
    refreshOriginByCurrentFrames(_anim.getFrameWidth(), _anim.getFrameHeight());
    _anim.applyTo(_sprite);
}

void cenemy::startDeath() {
    _sprite.setTexture(*_deathTex);
    _state = DEATH;
    _isDead = false; // will flip true when finished
    _anim.init(_deathFrameWidth, _deathFrameHeight, _deathSpeed, _deathFrames, /*loop*/false);
    refreshOriginByCurrentFrames(_anim.getFrameWidth(), _anim.getFrameHeight());
    _anim.applyTo(_sprite);
}

void cenemy::refreshOriginByCurrentFrames(int fw, int fh)
{
    _sprite.setOrigin(fw / 2.f, fh / 1.25f);
}

void cenemy::triggerAttack() {
    if (_state != WALK) return;
    startAttack();
}

void cenemy::updateAnimation(float deltaTime) {
    // If death already finalized, nothing to animate
    if (_state == DEATH && _isDead) return;

    _anim.update(deltaTime);
    _anim.applyTo(_sprite);

    if (_state == DEATH && _anim.isFinished())
        _isDead = true;

    else if (_state == ATTACK && _anim.isFinished())
        _isAttack = true;
}

void cenemy::faceLeft(EnemyType type) {
    if (type == RANGED_MECH)
        _sprite.setScale(-1.f, 1.f);
    else
        _sprite.setScale(-0.5f, 0.5f);
}

void cenemy::faceRight(EnemyType type) {
    if (type == RANGED_MECH)
        _sprite.setScale(1.f, 1.f);
    else
        _sprite.setScale(0.5f, 0.5f);
}

int cenemy::getHealthByType(EnemyType type) {
    switch (type) {
    case FAST_SCOUT: return 3;
    case RANGED_MECH: return 5;
    case HEAVY_WALKER: return 10;
    default: return 3;
    }
}

int cenemy::getSpeedByType(EnemyType type) {
    switch (type) {
    case FAST_SCOUT: return 120;
    case RANGED_MECH: return 100;
    case HEAVY_WALKER: return 70;
    default: return 70;
    }
}

int cenemy::getResourcesByType(EnemyType type) {
    switch (type) {
    case FAST_SCOUT: return 20;
    case RANGED_MECH: return 30;
    case HEAVY_WALKER: return 40;
    default: return 20;
    }
}

int cenemy::getDamageByType(EnemyType type)
{
    switch (type) {
    case FAST_SCOUT: return 1;
    case RANGED_MECH: return 2;
    case HEAVY_WALKER: return 3;
    default: return 100;
    }
}


