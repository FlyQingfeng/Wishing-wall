
import { createRouter, createWebHistory,createWebHashHistory } from 'vue-router'
import MVue from '../App.vue'
import Lobby from '../components/lobby.vue'
import Login from '../components/login.vue'
import Self from '../components/self.vue'
import Other from '../components/other.vue'
import About from '../components/about.vue'
import cs from '../components/search_box.vue'

const routes = [
  { path: '/', component: Login },
  { path: '/self', component: Self},
  { path: '/lobby', component: Lobby},
  { path: '/about', component: About},
  { path: '/other', component: Other},
  { path: '/cs', component: cs},
]

const router = createRouter({
  history: createWebHashHistory(),
  routes,
})

export default router
