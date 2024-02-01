<template>
    <div class="box">
        <div class="inputbox">
            <input :type="type" required v-model="value">
            <span>{{ placeholder }}</span>
        </div>
    </div>
</template>
<script>

export default {
    props: {
        type: {
            type: String,
            default: 'text',
        },
        modelValue: {
            type: [String, Number],
            default: '',
        },
        placeholder:{
            type:String,
            default:'输入提示',
        }
    },
    emits:['update:modelValue'],
    computed: {
        value: {
            get() {
                return this.modelValue
            },
            set(value) {
                this.$emit('update:modelValue', value)
            }
        }
    },
    data() {
        return {
            inputtext:'',
        };
    },
    methods:{
    }
}
</script>

<style scoped>
.box{
    width: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
    gap: 30px;
}
.inputbox{
    position:relative;
    width: 250px;
}
.inputbox input{ 
    width: 100%;
    border: 1px solid rgba(255, 255, 255, 0.25);
    border-radius: 5px;
    padding: 0.9rem 0.9rem;
    margin: 0.5rem 0;
    background: #647669;
    outline: none;
    color: #ffffff;
    font-size: 1em;
}
.inputbox span{
    position: absolute;
    left: 0;
    top: 0;
    padding: 0.9rem 0.9rem;
    margin: 0.5rem 0;
    pointer-events: none;
    font-size: 1em;
    color: rgba(255, 255, 255, 0.5);
    text-transform: uppercase;
    transition: 0.5s;
    z-index: 1;
}
.inputbox input:valid ~ span,
.inputbox input:focus ~ span{
    color: #00f732;
    transform:translateX(10px) translateY(-7px);
    font-size: 0.65em;
    padding: 0 10px;
    background-color: #6476696b;
    border-left:1px solid #00f732;
    border-right: 1px solid #00f732;
    letter-spacing: 0.2em;
}
.inputbox input:valid,
.inputbox input:focus{
    border: 1px solid #00f732;
}


</style>
