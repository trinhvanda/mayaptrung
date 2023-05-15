const wrapper = document.querySelector('.wrapper');
const loginLink = document.querySelector('.login-link');
const registerLink = document.querySelector('.register-link');
const btnPopup = document.querySelector('.nutdn');
const iconClose = document.querySelector('.icon-close');


registerLink.addEventListener('click', ()=> {
    wrapper.classList.add('active');
});

loginLink.addEventListener('click', ()=> {
    wrapper.classList.remove('active');
});

btnPopup.addEventListener('click', ()=> {
    wrapper.classList.add('active-popup');
});

iconClose.addEventListener('click', ()=> {
    wrapper.classList.remove('active-popup');
});

const $ = document.querySelector.bind(document);
const $$ = document.querySelectorAll.bind(document);

const tabs = $$(".tab-item");
const panes = $$(".thongtin");

const tabActive = $(".tab-item.active");
const line = $(".tabs .line");


tabs.forEach((tab, index) => {
const pane = panes[index];

tab.onclick = function () {
    $(".tab-item.active").classList.remove("active");
    $(".thongtin.active").classList.remove("active");
    
    // line.style.left = this.offsetLeft + "px";
    // line.style.width = this.offsetWidth + "px";

    this.classList.add("active");
    pane.classList.add("active");
    
};
});