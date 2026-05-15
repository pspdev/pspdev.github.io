---
title: Homebrew
layout: home
nav_order: 10
---

# Homebrew
{: .fs-8 .fw-700 .text-center }

This page contains a list of homebrew made for the PSP that can be searched through and downloaded.


Search:

<input id="search" oninput="search(value)">

<table id="pagination"></table>
<table id="homebrew_table">
</table>


<script type="text/javascript">
  let data = [];
  {% for item in site.data.homebrew.response.docs %}
    data.push({
      "name": "{{ item.title | escape }}",
      "description": "{{ item.description | escape }}",
      "author": "{{ item.creator | escape }}",
      "date": "{{ item.date | escape }}",
      "identifier": "{{ item.identifier | escape }}"
    });
  {% endfor %}
  const rowsPerPage = 20;
  let currentPage = 1;

  function updateTable(page, alternativeData = null) {
    if (!alternativeData)
      alternativeData = data;
    let firstEntry = (page - 1) * rowsPerPage;
    const table = document.getElementById("homebrew_table");
    table.innerHTML = `
      <tr>
          <th>Name</th>
          <th>Description</th>
          <th>Author</th>
      </tr>
    `;
    for(let i = firstEntry; i < alternativeData.length && i < firstEntry + rowsPerPage; i++) {
      const row = table.insertRow();
      const nameCell = row.insertCell(0);
      const descriptionCell = row.insertCell(1);
      const AuthorCell = row.insertCell(2);
      let nameLink = document.createElement("a");
      nameLink.href = "https://archive.org/details/" + alternativeData[i]["identifier"];
      nameLink.innerHTML = alternativeData[i]["name"];
      nameCell.appendChild(nameLink);
      descriptionCell.innerHTML = alternativeData[i]["description"];
      AuthorCell.innerHTML = alternativeData[i]["author"];
    }
    currentPage = page;
  }

  function addPageLink(parentElement, pageNumber, text, alternativeData = null) {
    if (!alternativeData)
      alternativeData = data;
    let div = document.createElement("div");
    let link = document.createElement("a");
    link.innerHTML = text;
    link.onclick=function() {
      updateTable(pageNumber, alternativeData);
      updatePaginationButtons(alternativeData);
    };
    link.href="javascript:void(0);";
    div.appendChild(link);
    parentElement.appendChild(div);
  }

  function updatePaginationButtons(alternativeData = null) {
    if (!alternativeData)
      alternativeData = data;
    const pagination = document.getElementById("pagination");
    pagination.innerHTML = "";
    const row = pagination.insertRow();
    let pageCount = Math.ceil(alternativeData.length/rowsPerPage);
    if (currentPage == 1) {
      let firstRow = row.insertCell();
      firstRow.innerHTML = "<< First";
      let previousRow = row.insertCell();
      previousRow.innerHTML = "< Previous";
    } else {
      addPageLink(row.insertCell(), 1, "<< First", alternativeData);
      addPageLink(row.insertCell(), currentPage - 1, "< Previous", alternativeData);
    }

    let currentPageRow = row.insertCell();
    currentPageRow.innerHTML = currentPage;

    if (currentPage == pageCount) {
      let nextRow = row.insertCell();
      nextRow.innerHTML = "Next >";
      let lastRow = row.insertCell();
      lastRow.innerHTML = "Last >>";
    } else {
      addPageLink(row.insertCell(), currentPage + 1, "Next >", alternativeData);
      addPageLink(row.insertCell(), pageCount, "Last >>", alternativeData);
    }
  }

  function search(text) {
    let newData = data.filter((item) =>
      item["name"].toLowerCase().includes(text.toLowerCase()) || 
      item["description"].toLowerCase().includes(text.toLowerCase()) ||
      item["author"].toLowerCase().includes(text.toLowerCase())
    );
    currentPage = 1;
    updateTable(1, newData);
    updatePaginationButtons(newData);
  }

  updateTable(currentPage);
  updatePaginationButtons();
</script>