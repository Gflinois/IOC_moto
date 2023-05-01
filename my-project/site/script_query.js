<script>
// Fetch the data from the file using AJAX
var xhr = new XMLHttpRequest();
xhr.onreadystatechange = function() {
    if (this.readyState === 4 && this.status === 200) {
        // Update the page content with the data from the file
        document.getElementById("<div id="dynamic-content"></div>dynamic-content").innerHTML = this.responseText;
    }
};
xhr.open("GET", "~/Sorbonne/IOC/IOC_moto/my_project/site/file.txt", true);
xhr.send();
</script>
