<template>
    <canvas ref="canvas"></canvas>
</template>
  
  <script>
  export default {
    mounted() {
      this.createRippleEffect();
    },
    methods: {
      createRippleEffect() {
        const canvas = this.$refs.canvas;
        const ctx = canvas.getContext('2d');
        

        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
  
        let ripples = [];
  
        function addRipple(event) {
          let x = event.x || event.clientX;
          let y = event.y || event.clientY;
          ripples.push({
            x: x,
            y: y,
            radius: 0,
            alpha: 1
          });
        }
        function createRipple() {
            let x = Math.random() * canvas.width;
            let y = Math.random() * canvas.height;
            ripples.push({
                x: x,
                y: y,
                radius: Math.random()*10+10,
                alpha: 1
            })
            
        }
        function animateRipples() {
          ctx.clearRect(0, 0, canvas.width, canvas.height);
          // 创建线性渐变
        const gradient = ctx.createLinearGradient(canvas.width/2, 0, canvas.width/2, canvas.height);
        // linear-gradient(1deg,#abdcff,#0396ff)
        // 添加颜色停止点
        gradient.addColorStop(0, '#abdcff');
        gradient.addColorStop(1, '#0396ff');
                
        // 设置填充样式为渐变
        ctx.fillStyle = gradient;
                
        // 填充背景
        ctx.fillRect(0, 0, canvas.width, canvas.height);
          for (let i = 0; i < ripples.length; i++) {
            let r = ripples[i];
            ctx.beginPath();
            ctx.arc(r.x, r.y, r.radius, 0, Math.PI * 2, false);
            ctx.fillStyle =`rgba(0, 100, 200, ${r.alpha})`;
            ctx.fill();
            r.y-=2;
            // r.radius++;
            ctx.strokeStyle = `rgba(0, 100, 200, ${r.alpha})`;
            ctx.stroke();
            r.alpha -= 0.007;
            if (r.alpha <= 0) {
              ripples.splice(i, 1);
            }
          }
          requestAnimationFrame(animateRipples);
        }
  
        // canvas.addEventListener('click', addRipple);
        // canvas.addEventListener('mousemove', addRipple);
        setInterval(createRipple, 100);  // 每隔 1000 毫秒创建一个新的水波纹
        animateRipples();
      }
    }
  }
  </script>
  
  <style scoped="">
  canvas {
    position: absolute;
    top: 0;
    left: 0;
  }
  </style>
  