(function() {

  const chapter = (n) => {
    return fetch(`/chapters/${n}.md`)
      .then(function(response) {
        return response.text();
      });
  };

  const source = () => {
    const numbers = ["01", "02", "03", "04"];
    return Promise.all(numbers.map(chapter))
      .then((result) => result.join("\n"));
  }

  document.addEventListener("DOMContentLoaded", (event) => {
    source()
      .then((source) => {
        remark.create({ source: source });
      });
  });

})();
