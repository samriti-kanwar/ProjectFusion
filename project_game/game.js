const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d");

let spaceship = { x: 270, y: 350, width: 50, height: 50 };
let bullets = [];
let enemies = [];
let score = 0;
let lives = 3;

// Load images
const spaceshipImg = new Image();
spaceshipImg.src = "spaceship.png";
const enemyImg = new Image();
enemyImg.src = "enemy.png";

// Load sound effects
const shootSound = new Audio("shoot.mp3");
const explosionSound = new Audio("explosion.mp3");
const gameOverSound = new Audio("gameover.mp3");

let leftPressed = false, rightPressed = false, spacePressed = false;

// Keyboard event listeners
document.addEventListener("keydown", (e) => {
    if (e.key === "ArrowLeft") leftPressed = true;
    if (e.key === "ArrowRight") rightPressed = true;
    if (e.key === " " && !spacePressed) {
        spacePressed = true;
        shoot();
    }
});
document.addEventListener("keyup", (e) => {
    if (e.key === "ArrowLeft") leftPressed = false;
    if (e.key === "ArrowRight") rightPressed = false;
    if (e.key === " ") spacePressed = false;
});

// Bullet shooting
function shoot() {
    bullets.push({ x: spaceship.x + 20, y: spaceship.y, width: 5, height: 10 });
    shootSound.play();
}

// Enemy spawning
function spawnEnemies() {
    let x = Math.random() * (canvas.width - 40);
    enemies.push({ x, y: 0, width: 40, height: 40 });
}

// Collision detection
function checkCollision(obj1, obj2) {
    return (
        obj1.x < obj2.x + obj2.width &&
        obj1.x + obj1.width > obj2.x &&
        obj1.y < obj2.y + obj2.height &&
        obj1.y + obj1.height > obj2.y
    );
}

// Game update function
function update() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Move spaceship
    if (leftPressed && spaceship.x > 0) spaceship.x -= 5;
    if (rightPressed && spaceship.x < canvas.width - spaceship.width) spaceship.x += 5;

    // Draw spaceship
    ctx.drawImage(spaceshipImg, spaceship.x, spaceship.y, spaceship.width, spaceship.height);

    // Move & draw bullets
    bullets.forEach((bullet, index) => {
        bullet.y -= 5;
        ctx.fillStyle = "yellow";
        ctx.fillRect(bullet.x, bullet.y, bullet.width, bullet.height);
        if (bullet.y < 0) bullets.splice(index, 1);
    });

    // Move & draw enemies
    enemies.forEach((enemy, index) => {
        enemy.y += 2;
        ctx.drawImage(enemyImg, enemy.x, enemy.y, enemy.width, enemy.height);

        // Check collision with bullets
        bullets.forEach((bullet, bulletIndex) => {
            if (checkCollision(bullet, enemy)) {
                enemies.splice(index, 1);
                bullets.splice(bulletIndex, 1);
                explosionSound.play();
                score += 10;
            }
        });

        // Check collision with spaceship
        if (checkCollision(enemy, spaceship)) {
            enemies.splice(index, 1);
            lives -= 1;
            if (lives === 0) {
                gameOverSound.play();
                alert("Game Over! Final Score: " + score);
                document.location.reload();
            }
        }
    });

    // Display score & lives
    ctx.fillStyle = "white";
    ctx.font = "16px Arial";
    ctx.fillText("Score: " + score, 10, 20);
    ctx.fillText("Lives: " + lives, 500, 20);

    requestAnimationFrame(update);
}

// Start game loop
setInterval(spawnEnemies, 1000);
update();
